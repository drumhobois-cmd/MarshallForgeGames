// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BECombatComponent.generated.h"

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

private:
	UPROPERTY(VisibleAnywhere, Category="Combat")
	bool bIsInFightingStance;
};
