// Fill out your copyright notice in the Description page of Project Settings.

#include "BECombatComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/SkeletalBodySetup.h"

UBECombatComponent::UBECombatComponent()
{
	// Tick is enabled only during an active named-body response (TG_PostPhysics) and disabled otherwise.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	bIsInFightingStance = false;
}

void UBECombatComponent::EnterFightingStance()
{
	bIsInFightingStance = true;
	UE_LOG(LogTemp, Log, TEXT("Fighting Stance: ON"));
}

void UBECombatComponent::ExitFightingStance()
{
	bIsInFightingStance = false;
	UE_LOG(LogTemp, Log, TEXT("Fighting Stance: OFF"));
}

void UBECombatComponent::ToggleFightingStance()
{
	if (bIsInFightingStance)
	{
		ExitFightingStance();
	}
	else
	{
		EnterFightingStance();
	}
}

bool UBECombatComponent::IsInFightingStance() const
{
	return bIsInFightingStance;
}

void UBECombatComponent::Jab()
{
	if (!JabMontage) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	USkeletalMeshComponent* Mesh = Character->GetMesh();
	if (!Mesh) return;

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (!AnimInstance) return;

	AnimInstance->Montage_Play(JabMontage);
}

void UBECombatComponent::BeginFistSweep(USkeletalMeshComponent* Mesh, FName SocketName)
{
	if (!Mesh) return;

	++CurrentWindowId;
	WindowTickCount = 0;
	HitFrameCandidateHitCount = 0;
	HitFrameSkeletalCandidateHitCount = 0;
	PreviousFistLocation = Mesh->GetSocketLocation(SocketName);
	bFistSweepActive = true;
	bHitLoggedThisWindow = false;
}

void UBECombatComponent::UpdateFistSweep(USkeletalMeshComponent* Mesh, FName SocketName, float FrameDeltaTime)
{
	if (!bFistSweepActive || !Mesh) return;

	UWorld* World = GetWorld();
	if (!World) return;

	const FVector CurrentFistLocation = Mesh->GetSocketLocation(SocketName);

	// Yellow sphere tracks the fist each frame
	DrawDebugSphere(World, CurrentFistLocation, 6.0f, 8, FColor::Yellow, false, -1.0f);

	// Orange line shows the path swept this frame
	DrawDebugLine(World, PreviousFistLocation, CurrentFistLocation, FColor::Orange, false, -1.0f, 0, 2.0f);

	// Swept-sphere query against Pawns only, ignoring the owning character
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	TArray<FHitResult> Hits;
	World->SweepMultiByObjectType(
		Hits,
		PreviousFistLocation,
		CurrentFistLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(6.0f),
		Params
	);

	++WindowTickCount;

	if (!bHitLoggedThisWindow)
	{
		// Raw result count and skeletal count (no actor filter; inspection only, does not alter ordering or selection)
		const int32 CandidateHitCount = Hits.Num();
		int32 SkeletalCandidateHitCount = 0;
		for (const FHitResult& CandHit : Hits)
		{
			if (Cast<USkeletalMeshComponent>(CandHit.GetComponent()))
			{
				++SkeletalCandidateHitCount;
			}
		}

		// Selection loop — identical ordering and actor-validity gate to baseline; indexed to record SelectedHitIndex
		for (int32 HitIdx = 0; HitIdx < Hits.Num(); ++HitIdx)
		{
			const FHitResult& Hit = Hits[HitIdx];
			if (Hit.GetActor())
			{
				UE_LOG(LogTemp, Log, TEXT("Fist contact: %s"), *Hit.GetActor()->GetName());
				bHitLoggedThisWindow = true;

				ACharacter* Target = Cast<ACharacter>(Hit.GetActor());
				if (Target)
				{
					// Shove direction: attacker -> target, horizontal only
					const FVector ShoveDir = (Target->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal2D();
					if (!ShoveDir.IsNearlyZero())
					{
						// --- BE-0003: exact Physics Asset body-geometry resolution from the capsule impact point ---
						// GetClosestPointOnPhysicsAsset (bApproximate=false) resolves the nearest PA body surface to
						// the capsule impact point. This is NOT a direct skeletal-mesh sweep hit; the sweep selected
						// Bob's capsule. Proximity, chain membership, and pre-recreate body validity are checked
						// before any physics-state change. The body pointer is re-acquired after RecreatePhysicsState
						// because the prior pointer may be stale after physics recreation.
						USkeletalMeshComponent* TargetSkel = Target->GetMesh();
						FName ResolvedBone = NAME_None;
						float ResolvedGeomDist = -1.0f;
						// Captured from FClosestPointOnPhysicsAsset for use in the PreApply sample (ClosestResult is inner-scoped)
						FVector ResolvedSurfacePoint = FVector::ZeroVector;
						FVector ResolvedSurfaceNormal = FVector::ZeroVector;

						if (TargetSkel)
						{
							FClosestPointOnPhysicsAsset ClosestResult;
							if (TargetSkel->GetClosestPointOnPhysicsAsset(Hit.ImpactPoint, ClosestResult, /*bApproximate=*/false))
							{
								// Distance is body-surface distance in world-space cm; 0 = fist is inside the body
								const float GeomDist = ClosestResult.Distance;
								const bool bWithinProximity = (GeomDist >= 0.0f) && (GeomDist <= MaxBodyResolutionRadiusCm);

								if (bWithinProximity && !ClosestResult.BoneName.IsNone())
								{
									// Chain membership: walk up from the resolved bone to UpperBodyChainRoot
									bool bInUpperChain = false;
									FName TestBone = ClosestResult.BoneName;
									while (!TestBone.IsNone())
									{
										if (TestBone == UpperBodyChainRoot) { bInUpperChain = true; break; }
										TestBone = TargetSkel->GetParentBone(TestBone);
									}

									if (bInUpperChain)
									{
										// Pre-check: verify a body instance exists before committing to physics recreation.
										// The pointer is intentionally not stored; it is re-acquired after recreation.
										FBodyInstance* PreBI = TargetSkel->GetBodyInstance(ClosestResult.BoneName);
										if (PreBI && PreBI->IsValidBodyInstance())
										{
											ResolvedBone = ClosestResult.BoneName;
											ResolvedGeomDist = GeomDist;
											ResolvedSurfacePoint = ClosestResult.ClosestWorldPosition;
											ResolvedSurfaceNormal = ClosestResult.Normal;
										}
									}
								}
							}
						}

						// Guard: finite, strictly positive duration required before any collision or simulation change
						const bool bDurationValid = FMath::IsFinite(UpperBodyReactionDurationSecs) && UpperBodyReactionDurationSecs > 0.0f;
						if (!ResolvedBone.IsNone() && !bDurationValid)
						{
							UE_LOG(LogTemp, Warning,
								TEXT("BE_UPPER_BODY_RESPONSE_V1 | Phase=Skipped | WindowId=%u | Reason=InvalidDuration | Duration=%.4f"),
								CurrentWindowId, UpperBodyReactionDurationSecs);
						}
						const bool bNamedBodyEligible = !ResolvedBone.IsNone() && bDurationValid;

						if (bNamedBodyEligible)
						{
							// Cancel any pending restore from a previous hit before re-applying
							if (PhysicsRestoreHandle.IsValid())
							{
								ExecutePhysicsRestore();
							}

							// Upgrade collision and recreate physics state so Chaos creates simulation actors
							const ECollisionEnabled::Type OrigCollision = TargetSkel->GetCollisionEnabled();
							TargetSkel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
							TargetSkel->RecreatePhysicsState();

							// Enable physics on the upper-body chain from the chain root
							TargetSkel->SetAllBodiesBelowSimulatePhysics(UpperBodyChainRoot, true, true);

							// Re-acquire the body instance after physics recreation; the pre-resolution pointer is stale
							FBodyInstance* PostRecreateBody = TargetSkel->GetBodyInstance(ResolvedBone);
							const bool bPostBodyValid = (PostRecreateBody != nullptr) && PostRecreateBody->IsValidBodyInstance();

							if (bPostBodyValid)
							{
								const FVector AppliedVelocity = ShoveDir * UpperBodyResponseVelocity;

								// --- BE_RESPONSE_SAMPLE_V1 PreApply ---
								// Emitted after valid body re-acquisition and before the velocity request.
								{
									// Kinematic fist velocity estimate: (SweepEnd - SweepStart) / TickDelta
									FString FistVelStr;
									if (FMath::IsFinite(FrameDeltaTime) && FrameDeltaTime > 0.0f)
									{
										const FVector FistVelEst = (CurrentFistLocation - PreviousFistLocation) / FrameDeltaTime;
										if (FistVelEst.ContainsNaN())
											FistVelStr = TEXT("Unavailable (NaN)");
										else
											FistVelStr = FString::Printf(TEXT("(%.2f,%.2f,%.2f) cm/s (kinematic path-estimate; not strike velocity)"),
												FistVelEst.X, FistVelEst.Y, FistVelEst.Z);
									}
									else
									{
										FistVelStr = FMath::IsFinite(FrameDeltaTime)
											? FString::Printf(TEXT("Unavailable (non-positive delta: %.6f s)"), FrameDeltaTime)
											: TEXT("Unavailable (non-finite delta)");
									}

									// Solver mass: raw single-body value; not chain or effective mass
									const float SolverMassKg = PostRecreateBody->GetBodyMass();
									const bool bMassUsable = FMath::IsFinite(SolverMassKg) && SolverMassKg > 0.0f;
									const FString MassStr = bMassUsable
										? FString::Printf(TEXT("%.4f kg (FBodyInstance::GetBodyMass; solver value, not chain/effective mass)"), SolverMassKg)
										: FString::Printf(TEXT("Unavailable (non-positive: %.4f)"), SolverMassKg);

									// Body state before velocity request
									const FTransform BodyTransform = PostRecreateBody->GetUnrealWorldTransform();
									const FVector BodyPos = BodyTransform.GetLocation();
									const FQuat BodyRot = BodyTransform.GetRotation();
									const FVector LinVel = PostRecreateBody->GetUnrealWorldVelocity();
									const FVector AngVelRad = PostRecreateBody->GetUnrealWorldAngularVelocityInRadians();
									// Velocity at the resolved PA surface point (world-space, body is at rest pre-apply; stored as body-local for PostPhysics tracking)
									const FVector PointVel = PostRecreateBody->GetUnrealWorldVelocityAtPoint(ResolvedSurfacePoint);

									// Requested solver impulse equivalent: mass * |delta-v|; not a contact impulse
									const FString ImpEquivStr = bMassUsable
										? FString::Printf(TEXT("%.2f kg*cm/s (%.4f N*s) (requested solver impulse equiv; not contact impulse)"),
											SolverMassKg * UpperBodyResponseVelocity,
											(SolverMassKg * UpperBodyResponseVelocity) / 100.0f)
										: TEXT("Unavailable (mass unavailable)");

									UE_LOG(LogTemp, Log,
										TEXT("BE_RESPONSE_SAMPLE_V1 | Phase=PreApply | WindowId=%u")
										TEXT(" | FistVelEst=%s")
										TEXT(" | PA_SurfacePoint=(%.2f,%.2f,%.2f) cm | PA_Normal=(%.4f,%.4f,%.4f) unitless | PA_Body=%s | PA_SurfaceDist=%.2f cm | PA_SurfacePointTracking=body-local-derived")
										TEXT(" | SolverMass=%s")
										TEXT(" | BodyPos=(%.2f,%.2f,%.2f) cm | BodyRot=(%.4f,%.4f,%.4f,%.4f)")
										TEXT(" | LinVel=(%.2f,%.2f,%.2f) cm/s | AngVelRad=(%.4f,%.4f,%.4f) rad/s")
										TEXT(" | PointVel=(%.2f,%.2f,%.2f) cm/s")
										TEXT(" | RequestedDeltaV=(%.2f,%.2f,%.2f) cm/s | RequestedDir=(%.4f,%.4f,%.4f) unitless | bVelChange=true | ConfiguredDuration=%.4f s")
										TEXT(" | RequestedSolverImpulseEquiv=%s"),
										CurrentWindowId,
										*FistVelStr,
										ResolvedSurfacePoint.X, ResolvedSurfacePoint.Y, ResolvedSurfacePoint.Z,
										ResolvedSurfaceNormal.X, ResolvedSurfaceNormal.Y, ResolvedSurfaceNormal.Z,
										*ResolvedBone.ToString(), ResolvedGeomDist,
										*MassStr,
										BodyPos.X, BodyPos.Y, BodyPos.Z,
										BodyRot.X, BodyRot.Y, BodyRot.Z, BodyRot.W,
										LinVel.X, LinVel.Y, LinVel.Z,
										AngVelRad.X, AngVelRad.Y, AngVelRad.Z,
										PointVel.X, PointVel.Y, PointVel.Z,
										AppliedVelocity.X, AppliedVelocity.Y, AppliedVelocity.Z,
										ShoveDir.X, ShoveDir.Y, ShoveDir.Z,
										UpperBodyReactionDurationSecs,
										*ImpEquivStr
									);

									// Arm bounded TG_PostPhysics sampling for the duration of this response
									ActiveResponseResolvedPoint = ResolvedSurfacePoint;
									// R2: convert surface point to body-local space so PostPhysics ticks can track the material point
									ActiveResponseLocalPoint = BodyTransform.InverseTransformPosition(ResolvedSurfacePoint);
									ActiveResponseRequestedDir = ShoveDir;
									ResponseSamplingMesh = TargetSkel;
									ResponseSamplingBone = ResolvedBone;
									ResponseSamplingWindowId = CurrentWindowId;
									ResponseSamplingStartTime = World->GetTimeSeconds();
									ResponseSamplingConfiguredDuration = UpperBodyReactionDurationSecs;
									ResponseSampleOrdinal = 0;
									bResponseSamplingActive = true;
									SetComponentTickEnabled(true);
								}
								// --- end BE_RESPONSE_SAMPLE_V1 PreApply ---

								// Apply velocity change to the resolved body (bVelChange=true ignores mass)
								PostRecreateBody->AddImpulse(AppliedVelocity, true);

								// Record restore state
								PendingRestoreMesh = TargetSkel;
								ActiveRestoreChainRoot = UpperBodyChainRoot;
								ActiveRestoreCollision = OrigCollision;

								const uint32 WinId = CurrentWindowId;
								const FName CapturedBone = ResolvedBone;
								const FName CapturedRoot = UpperBodyChainRoot;
								TWeakObjectPtr<UBECombatComponent> WeakSelf(this);

								// Schedule bounded restoration; lambda delegates to ExecutePhysicsRestore for safe state management
								GetWorld()->GetTimerManager().SetTimer(
									PhysicsRestoreHandle,
									FTimerDelegate::CreateLambda(
										[WeakSelf, WinId, CapturedBone, CapturedRoot]()
										{
											if (UBECombatComponent* SelfPtr = WeakSelf.Get())
											{
												const bool bMeshWasValid = SelfPtr->PendingRestoreMesh.IsValid();
												SelfPtr->ExecutePhysicsRestore();
												if (bMeshWasValid)
												{
													UE_LOG(LogTemp, Log,
														TEXT("BE_UPPER_BODY_RESPONSE_V1 | Phase=Restored | WindowId=%u | Bone=%s | ChainRoot=%s | PhysicsDisabled=true | CollisionRestored=true"),
														WinId, *CapturedBone.ToString(), *CapturedRoot.ToString());
												}
												else
												{
													UE_LOG(LogTemp, Log,
														TEXT("BE_UPPER_BODY_RESPONSE_V1 | Phase=RestoreSkipped | WindowId=%u | Reason=MeshGone"),
														WinId);
												}
											}
										}),
									UpperBodyReactionDurationSecs,
									false
								);

								UE_LOG(LogTemp, Log,
									TEXT("BE_UPPER_BODY_RESPONSE_V1 | Phase=Applied | WindowId=%u | Branch=NamedBody")
									TEXT(" | ResolvedBone=%s | GeomDist=%.1f cm | ResolutionSource=GetClosestPointOnPhysicsAsset_exact_from_capsule_impact")
									TEXT(" | ChainRoot=%s | MaxProximityRadius=%.1f cm | ChainVerified=true | PostRecreateBodyValid=true")
									TEXT(" | AppliedVelocity=(%.1f,%.1f,%.1f) cm/s | bVelChange=true | ReactionDurationSecs=%.2f")
									TEXT(" | CollisionUpgraded=QueryOnly->QueryAndPhysics"),
									CurrentWindowId, *ResolvedBone.ToString(), ResolvedGeomDist,
									*UpperBodyChainRoot.ToString(), MaxBodyResolutionRadiusCm,
									AppliedVelocity.X, AppliedVelocity.Y, AppliedVelocity.Z,
									UpperBodyReactionDurationSecs);
							}
							else
							{
								// Post-recreate body invalid: undo physics-state changes immediately and execute capsule fallback
								TargetSkel->SetAllBodiesBelowSimulatePhysics(UpperBodyChainRoot, false, true);
								TargetSkel->SetCollisionEnabled(OrigCollision);
								TargetSkel->RecreatePhysicsState();

								Target->LaunchCharacter(ShoveDir * 250.0f, true, false);

								UE_LOG(LogTemp, Log,
									TEXT("BE_UPPER_BODY_RESPONSE_V1 | Phase=Applied | WindowId=%u | Branch=CapsuleFallback | Reason=PostRecreateBodyInvalid | ResolvedBone=%s | LaunchVelocity=250.0 cm/s"),
									CurrentWindowId, *ResolvedBone.ToString());
							}
						}
						else
						{
							// Capsule fallback: no eligible named body resolved or invalid duration
							Target->LaunchCharacter(ShoveDir * 250.0f, true, false);

							UE_LOG(LogTemp, Log,
								TEXT("BE_UPPER_BODY_RESPONSE_V1 | Phase=Applied | WindowId=%u | Branch=CapsuleFallback | Reason=NoNamedBodyResolved | LaunchVelocity=250.0 cm/s"),
								CurrentWindowId);
						}
					}
				}

				// --- BE_CONTACT_SAMPLE_V1 ---
				HitFrameCandidateHitCount = CandidateHitCount;
				HitFrameSkeletalCandidateHitCount = SkeletalCandidateHitCount;

				const FString AttackerStr = GetOwner() ? GetOwner()->GetName() : TEXT("Unavailable");
				UPrimitiveComponent* SelectedComp = Hit.GetComponent();
				const FString CompName  = SelectedComp ? SelectedComp->GetName() : TEXT("Unavailable");
				const FString CompClass = SelectedComp ? SelectedComp->GetClass()->GetName() : TEXT("Unavailable");
				const FString BoneStr   = Hit.BoneName.IsNone() ? TEXT("None") : Hit.BoneName.ToString();

				// TickDelta: reportable only when finite and strictly positive; zero/negative/non-finite → Unavailable
				FString TickDeltaStr;
				if (!FMath::IsFinite(FrameDeltaTime))
					TickDeltaStr = TEXT("Unavailable (non-finite)");
				else if (FrameDeltaTime <= 0.0f)
					TickDeltaStr = FString::Printf(TEXT("Unavailable (non-positive: %.6f s)"), FrameDeltaTime);
				else
					TickDeltaStr = FString::Printf(TEXT("%.6f s"), FrameDeltaTime);

				// Path-speed: double precision avoids float narrowing; guard delta > 0, finite non-negative distance and quotient
				FString SpeedStr;
				if (FMath::IsFinite(FrameDeltaTime) && FrameDeltaTime > 0.0f)
				{
					const double PathDist = static_cast<double>(FVector::Dist(PreviousFistLocation, CurrentFistLocation));
					if (FMath::IsFinite(PathDist) && PathDist >= 0.0)
					{
						const double PathSpeedCmS = PathDist / static_cast<double>(FrameDeltaTime);
						if (FMath::IsFinite(PathSpeedCmS) && PathSpeedCmS >= 0.0)
							SpeedStr = FString::Printf(TEXT("%.1f cm/s (kinematic path-speed estimate)"), PathSpeedCmS);
						else
							SpeedStr = TEXT("Unavailable (non-finite quotient)");
					}
					else
					{
						SpeedStr = TEXT("Unavailable (non-finite path distance)");
					}
				}
				else
				{
					SpeedStr = TEXT("Unavailable (invalid delta)");
				}

				// Component-level simulation: Unavailable if null component
				const FString CompSimStr = SelectedComp
					? (SelectedComp->IsSimulatingPhysics() ? TEXT("true") : TEXT("false"))
					: TEXT("Unavailable");

				// Named-body simulation and mass: require skeletal component, non-None BoneName, valid body instance
				FString BodySimStr;
				FString MassStr;
				USkeletalMeshComponent* SkelComp = Cast<USkeletalMeshComponent>(SelectedComp);
				if (!SkelComp)
				{
					BodySimStr = TEXT("Unavailable (not skeletal)");
					MassStr    = TEXT("Unavailable (not skeletal)");
				}
				else if (Hit.BoneName.IsNone())
				{
					BodySimStr = TEXT("Unavailable (BoneName None)");
					MassStr    = TEXT("Unavailable (BoneName None)");
				}
				else
				{
					FBodyInstance* BodyInst = SkelComp->GetBodyInstance(Hit.BoneName);
					if (!BodyInst)
					{
						BodySimStr = TEXT("Unavailable (no body instance)");
						MassStr    = TEXT("Unavailable (no body instance)");
					}
					else if (!BodyInst->IsValidBodyInstance())
					{
						BodySimStr = TEXT("Unavailable (invalid body instance)");
						MassStr    = TEXT("Unavailable (invalid body instance)");
					}
					else
					{
						BodySimStr = SkelComp->IsSimulatingPhysics(Hit.BoneName) ? TEXT("true") : TEXT("false");
						const float BoneMassKg = SkelComp->GetBoneMass(Hit.BoneName, true);
						if (FMath::IsFinite(BoneMassKg) && BoneMassKg > 0.0f)
							MassStr = FString::Printf(TEXT("%.2f kg (source: USkeletalMeshComponent::GetBoneMass)"), BoneMassKg);
						else
							MassStr = FString::Printf(TEXT("Unavailable (non-positive mass: %.4f)"), BoneMassKg);
					}
				}

				UE_LOG(LogTemp, Log,
					TEXT("BE_CONTACT_SAMPLE_V1 | WindowId=%u | Attacker=%s | CandidateHitCount=%d | SkeletalCandidateHitCount=%d | SelectedHitIndex=%d")
					TEXT(" | Actor=%s | Component=%s | ComponentClass=%s | BoneName=%s")
					TEXT(" | SweepStart=(%.1f,%.1f,%.1f) cm | SweepEnd=(%.1f,%.1f,%.1f) cm | SweepRadius=6.0 cm")
					TEXT(" | TickDelta=%s | PathSpeedEstimate=%s")
					TEXT(" | ImpactPoint=(%.1f,%.1f,%.1f) cm | ImpactNormal=(%.3f,%.3f,%.3f) unitless")
					TEXT(" | BlockingHit=%s | StartPenetrating=%s")
					TEXT(" | ComponentSimulatingPhysics=%s | BodySimulatingPhysics=%s | BodyMass=%s"),
					CurrentWindowId, *AttackerStr,
					CandidateHitCount, SkeletalCandidateHitCount, HitIdx,
					*Hit.GetActor()->GetName(), *CompName, *CompClass, *BoneStr,
					PreviousFistLocation.X, PreviousFistLocation.Y, PreviousFistLocation.Z,
					CurrentFistLocation.X, CurrentFistLocation.Y, CurrentFistLocation.Z,
					*TickDeltaStr, *SpeedStr,
					Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z,
					Hit.ImpactNormal.X, Hit.ImpactNormal.Y, Hit.ImpactNormal.Z,
					Hit.bBlockingHit ? TEXT("true") : TEXT("false"),
					Hit.bStartPenetrating ? TEXT("true") : TEXT("false"),
					*CompSimStr, *BodySimStr, *MassStr
				);
				// --- end BE_CONTACT_SAMPLE_V1 ---

				break;
			}
		}
	}

	PreviousFistLocation = CurrentFistLocation;
}

void UBECombatComponent::EndFistSweep()
{
	if (!bFistSweepActive) return;

	const FString AttackerStr = GetOwner() ? GetOwner()->GetName() : TEXT("Unavailable");
	UE_LOG(LogTemp, Log,
		TEXT("BE_CONTACT_WINDOW_V1 | WindowId=%u | Attacker=%s | Result=%s | SampleCount=%d | TotalTicks=%d | HitFrameCandidateHitCount=%d | HitFrameSkeletalCandidateHitCount=%d"),
		CurrentWindowId, *AttackerStr,
		bHitLoggedThisWindow ? TEXT("HIT") : TEXT("MISS"),
		bHitLoggedThisWindow ? 1 : 0,
		WindowTickCount,
		bHitLoggedThisWindow ? HitFrameCandidateHitCount : 0,
		bHitLoggedThisWindow ? HitFrameSkeletalCandidateHitCount : 0
	);

	bFistSweepActive = false;
	bHitLoggedThisWindow = false;
	WindowTickCount = 0;
	HitFrameCandidateHitCount = 0;
	HitFrameSkeletalCandidateHitCount = 0;
}

void UBECombatComponent::ClearResponseSamplingState()
{
	bResponseSamplingActive = false;
	ResponseSamplingMesh.Reset();
	ResponseSamplingBone = NAME_None;
	ResponseSamplingWindowId = 0;
	ResponseSamplingStartTime = 0.0f;
	ResponseSampleOrdinal = 0;
	ResponseSamplingConfiguredDuration = 0.0f;
	ActiveResponseResolvedPoint = FVector::ZeroVector;
	ActiveResponseLocalPoint = FVector::ZeroVector;
	ActiveResponseRequestedDir = FVector::ZeroVector;
	SetComponentTickEnabled(false);
}

void UBECombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bResponseSamplingActive) return;

	UWorld* World = GetWorld();
	if (!World) { ClearResponseSamplingState(); return; }

	const float Elapsed = World->GetTimeSeconds() - ResponseSamplingStartTime;

	// Safety net: stop sampling once elapsed time clearly exceeds the configured response window.
	// Normal path: ExecutePhysicsRestore disables the tick before this guard triggers.
	if (Elapsed > ResponseSamplingConfiguredDuration + 0.1f)
	{
		ClearResponseSamplingState();
		return;
	}

	USkeletalMeshComponent* SkelMesh = ResponseSamplingMesh.Get();
	if (!SkelMesh) { ClearResponseSamplingState(); return; }

	FBodyInstance* Body = SkelMesh->GetBodyInstance(ResponseSamplingBone);
	const bool bBodyValid = (Body != nullptr) && Body->IsValidBodyInstance();
	// USkeletalMeshComponent::IsSimulatingPhysics uses the same module as the existing contact sample; avoids PhysicsCore dependency
	const bool bBodySimulating = bBodyValid && SkelMesh->IsSimulatingPhysics(ResponseSamplingBone);

	// R1: use explicit unavailable strings when body is invalid; zero is plausible data, not an absence marker.
	// R2: re-derive the tracked surface point from body-local coordinates each sample so it follows the body.
	FString BodyPosStr, BodyRotStr, LinVelStr, AngVelStr, TrackedPtStr, PointVelStr, DirProjStr;
	if (bBodyValid)
	{
		const FTransform BodyTransform = Body->GetUnrealWorldTransform();
		const FVector BodyPos = BodyTransform.GetLocation();
		const FQuat BodyRot = BodyTransform.GetRotation();
		const FVector LinVel = Body->GetUnrealWorldVelocity();
		const FVector AngVelRad = Body->GetUnrealWorldAngularVelocityInRadians();
		// Re-derive the tracked surface point in world space from body-local coordinates captured at PreApply
		const FVector TrackedWorldPoint = BodyTransform.TransformPosition(ActiveResponseLocalPoint);
		const FVector PointVel = Body->GetUnrealWorldVelocityAtPoint(TrackedWorldPoint);
		const float DirProj = FVector::DotProduct(LinVel, ActiveResponseRequestedDir);

		BodyPosStr   = FString::Printf(TEXT("(%.2f,%.2f,%.2f) cm"), BodyPos.X, BodyPos.Y, BodyPos.Z);
		BodyRotStr   = FString::Printf(TEXT("(%.4f,%.4f,%.4f,%.4f)"), BodyRot.X, BodyRot.Y, BodyRot.Z, BodyRot.W);
		LinVelStr    = FString::Printf(TEXT("(%.2f,%.2f,%.2f) cm/s"), LinVel.X, LinVel.Y, LinVel.Z);
		AngVelStr    = FString::Printf(TEXT("(%.4f,%.4f,%.4f) rad/s"), AngVelRad.X, AngVelRad.Y, AngVelRad.Z);
		TrackedPtStr = FString::Printf(TEXT("(%.2f,%.2f,%.2f) cm"), TrackedWorldPoint.X, TrackedWorldPoint.Y, TrackedWorldPoint.Z);
		PointVelStr  = FString::Printf(TEXT("(%.2f,%.2f,%.2f) cm/s"), PointVel.X, PointVel.Y, PointVel.Z);
		DirProjStr   = FString::Printf(TEXT("%.2f cm/s"), DirProj);
	}
	else
	{
		const FString U(TEXT("Unavailable (invalid body instance)"));
		BodyPosStr = BodyRotStr = LinVelStr = AngVelStr = TrackedPtStr = PointVelStr = DirProjStr = U;
	}

	UE_LOG(LogTemp, Log,
		TEXT("BE_RESPONSE_SAMPLE_V1 | Phase=PostPhysics | WindowId=%u | Ordinal=%d | ElapsedSecs=%.4f s | ConfiguredDuration=%.4f s")
		TEXT(" | BodyValid=%s | BodySimulating=%s")
		TEXT(" | BodyPos=%s | BodyRot=%s")
		TEXT(" | LinVel=%s | AngVelRad=%s")
		TEXT(" | TrackedSurfacePoint=%s | PointVel=%s | LinVelAlongResponseDir=%s"),
		ResponseSamplingWindowId, ResponseSampleOrdinal, Elapsed, ResponseSamplingConfiguredDuration,
		bBodyValid ? TEXT("true") : TEXT("false"),
		bBodySimulating ? TEXT("true") : TEXT("false"),
		*BodyPosStr, *BodyRotStr,
		*LinVelStr, *AngVelStr,
		*TrackedPtStr, *PointVelStr, *DirProjStr
	);

	++ResponseSampleOrdinal;
}

void UBECombatComponent::ExecutePhysicsRestore()
{
	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(PhysicsRestoreHandle);
	}
	PhysicsRestoreHandle.Invalidate();
	if (PendingRestoreMesh.IsValid())
	{
		PendingRestoreMesh->SetAllBodiesBelowSimulatePhysics(ActiveRestoreChainRoot, false, true);
		PendingRestoreMesh->SetCollisionEnabled(ActiveRestoreCollision);
		PendingRestoreMesh->RecreatePhysicsState();
	}
	PendingRestoreMesh.Reset();
	ClearResponseSamplingState();
}

void UBECombatComponent::OnUnregister()
{
	UWorld* W = GetWorld();
	if (W && !W->bIsTearingDown)
	{
		// Active world: restore Bob's physics so the reaction does not outlive the attacker component
		ExecutePhysicsRestore();
	}
	else
	{
		// World tearing down or null: skip physics mutation to avoid Invalid Bodies; cancel timer and clear state only
		if (W) { W->GetTimerManager().ClearTimer(PhysicsRestoreHandle); }
		PhysicsRestoreHandle.Invalidate();
		PendingRestoreMesh.Reset();
		// Clear all sampling fields directly; SetComponentTickEnabled is not safe during teardown (R3)
		bResponseSamplingActive = false;
		ResponseSamplingMesh.Reset();
		ResponseSamplingBone = NAME_None;
		ResponseSamplingWindowId = 0;
		ResponseSamplingStartTime = 0.0f;
		ResponseSampleOrdinal = 0;
		ResponseSamplingConfiguredDuration = 0.0f;
		ActiveResponseResolvedPoint = FVector::ZeroVector;
		ActiveResponseLocalPoint = FVector::ZeroVector;
		ActiveResponseRequestedDir = FVector::ZeroVector;
	}
	Super::OnUnregister();
}
