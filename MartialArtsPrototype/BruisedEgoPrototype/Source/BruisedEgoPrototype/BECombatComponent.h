// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "BECombatComponent.generated.h"

class UAnimMontage;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(BruisedEgo), meta=(BlueprintSpawnableComponent))
class BRUISEDEGOPROTOTYPE_API UBECombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBECombatComponent();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void EnterFightingStance();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void ExitFightingStance();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void ToggleFightingStance();

	UFUNCTION(BlueprintPure, Category="Combat")
	bool IsInFightingStance() const;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void Jab();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UAnimMontage> JabMontage;

	// BE-0003 experiment parameters — tunable in the Details panel without a rebuild
	UPROPERTY(EditAnywhere, Category="Combat|Experiment", meta=(ClampMin="0.01"))
	float UpperBodyReactionDurationSecs = 0.4f;

	UPROPERTY(EditAnywhere, Category="Combat|Experiment")
	float UpperBodyResponseVelocity = 200.0f;

	UPROPERTY(EditAnywhere, Category="Combat|Experiment")
	FName UpperBodyChainRoot = TEXT("spine_03");

	UPROPERTY(EditAnywhere, Category="Combat|Experiment", meta=(ClampMin="1.0"))
	float MaxBodyResolutionRadiusCm = 50.0f;

	// Fist sweep window — called by UBEAnimNotifyState_JabWindow
	void BeginFistSweep(USkeletalMeshComponent* Mesh, FName SocketName);
	void UpdateFistSweep(USkeletalMeshComponent* Mesh, FName SocketName, float FrameDeltaTime);
	void EndFistSweep();

protected:
	virtual void OnUnregister() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Cancels any pending restore timer, restores Bob's physics/collision, and clears state.
	// Only call during an active world; skip during teardown (bIsTearingDown) to avoid Invalid Bodies.
	void ExecutePhysicsRestore();

	// Disables the PostPhysics tick and resets all BE-0004 sampling fields.
	// Safe to call any time the response ends (restore, re-hit cancel, unregister, teardown path excluded).
	void ClearResponseSamplingState();

	UPROPERTY(VisibleAnywhere, Category="Combat")
	bool bIsInFightingStance;

	bool bFistSweepActive = false;
	FVector PreviousFistLocation = FVector::ZeroVector;
	bool bHitLoggedThisWindow = false;

	uint32 CurrentWindowId = 0;
	int32 WindowTickCount = 0;
	int32 HitFrameCandidateHitCount = 0;
	int32 HitFrameSkeletalCandidateHitCount = 0;

	// BE-0003 upper-body restore state
	TWeakObjectPtr<USkeletalMeshComponent> PendingRestoreMesh;
	FTimerHandle PhysicsRestoreHandle;
	FName ActiveRestoreChainRoot;
	ECollisionEnabled::Type ActiveRestoreCollision = ECollisionEnabled::QueryOnly;

	// BE-0004 response observability sampling state — active only during the named-body response window
	bool bResponseSamplingActive = false;
	TWeakObjectPtr<USkeletalMeshComponent> ResponseSamplingMesh;
	FName ResponseSamplingBone;
	uint32 ResponseSamplingWindowId = 0;
	float ResponseSamplingStartTime = 0.0f;
	int32 ResponseSampleOrdinal = 0;
	float ResponseSamplingConfiguredDuration = 0.0f;
	// World-space point captured at impact (for PreApply log; not used for PostPhysics queries)
	FVector ActiveResponseResolvedPoint = FVector::ZeroVector;
	// Body-local coordinates of the resolved PA surface point, captured at PreApply.
	// Re-derived to world space each PostPhysics tick via the current body transform (R2).
	FVector ActiveResponseLocalPoint = FVector::ZeroVector;
	// Unit direction of the applied velocity request (for projection observables)
	FVector ActiveResponseRequestedDir = FVector::ZeroVector;
};
