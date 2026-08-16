// Fill out your copyright notice in the Description page of Project Settings.

#include "BECombatComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

UBECombatComponent::UBECombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

	PreviousFistLocation = Mesh->GetSocketLocation(SocketName);
	bFistSweepActive = true;
	bHitLoggedThisWindow = false;
}

void UBECombatComponent::UpdateFistSweep(USkeletalMeshComponent* Mesh, FName SocketName)
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

	if (!bHitLoggedThisWindow)
	{
		for (const FHitResult& Hit : Hits)
		{
			if (Hit.GetActor())
			{
				UE_LOG(LogTemp, Log, TEXT("Fist contact: %s"), *Hit.GetActor()->GetName());
				bHitLoggedThisWindow = true;
				break;
			}
		}
	}

	PreviousFistLocation = CurrentFistLocation;
}

void UBECombatComponent::EndFistSweep()
{
	bFistSweepActive = false;
	bHitLoggedThisWindow = false;
}
