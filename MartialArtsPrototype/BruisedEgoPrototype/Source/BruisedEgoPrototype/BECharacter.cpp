// Fill out your copyright notice in the Description page of Project Settings.

#include "BECharacter.h"
#include "BECombatComponent.h"

ABECharacter::ABECharacter()
{
	CombatComponent = CreateDefaultSubobject<UBECombatComponent>(TEXT("CombatComponent"));
}

bool ABECharacter::IsInFightingStance() const
{
	return CombatComponent->IsInFightingStance();
}
