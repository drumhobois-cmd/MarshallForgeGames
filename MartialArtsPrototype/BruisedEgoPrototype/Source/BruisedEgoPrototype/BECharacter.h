// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BECharacter.generated.h"

class UBECombatComponent;

UCLASS()
class BRUISEDEGOPROTOTYPE_API ABECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABECharacter();

	UFUNCTION(BlueprintPure, Category="Combat")
	bool IsInFightingStance() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UBECombatComponent> CombatComponent;
};
