// Fill out your copyright notice in the Description page of Project Settings.

#include "BECharacter.h"
#include "BECombatComponent.h"
#include "EnhancedInputComponent.h"

ABECharacter::ABECharacter()
{
	CombatComponent = CreateDefaultSubobject<UBECombatComponent>(TEXT("CombatComponent"));
}

bool ABECharacter::IsInFightingStance() const
{
	return CombatComponent->IsInFightingStance();
}

void ABECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (FightingStanceAction)
		{
			EIC->BindAction(FightingStanceAction, ETriggerEvent::Started, this, &ABECharacter::OnFightingStanceTriggered);
		}
		if (JabAction)
		{
			EIC->BindAction(JabAction, ETriggerEvent::Started, this, &ABECharacter::OnJabStarted);
		}
	}
}

void ABECharacter::OnFightingStanceTriggered()
{
	CombatComponent->ToggleFightingStance();
}

void ABECharacter::OnJabStarted()
{
	CombatComponent->Jab();
}
