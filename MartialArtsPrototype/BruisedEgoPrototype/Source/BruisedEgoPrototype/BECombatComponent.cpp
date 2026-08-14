// Fill out your copyright notice in the Description page of Project Settings.

#include "BECombatComponent.h"

UBECombatComponent::UBECombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsInFightingStance = false;
}

void UBECombatComponent::EnterFightingStance()
{
	bIsInFightingStance = true;
}

void UBECombatComponent::ExitFightingStance()
{
	bIsInFightingStance = false;
}

void UBECombatComponent::ToggleFightingStance()
{
	bIsInFightingStance = !bIsInFightingStance;
}

bool UBECombatComponent::IsInFightingStance() const
{
	return bIsInFightingStance;
}
