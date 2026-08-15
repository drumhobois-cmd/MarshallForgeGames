// Fill out your copyright notice in the Description page of Project Settings.

#include "BECombatComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

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
