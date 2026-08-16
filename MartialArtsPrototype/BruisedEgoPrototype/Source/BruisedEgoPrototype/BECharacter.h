// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BECharacter.generated.h"

class UBECombatComponent;
class UInputAction;

UCLASS()
class BRUISEDEGOPROTOTYPE_API ABECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABECharacter();

	UFUNCTION(BlueprintPure, Category="Combat")
	bool IsInFightingStance() const;

	UBECombatComponent* GetCombatComponent() const { return CombatComponent; }

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> FightingStanceAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> JabAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UBECombatComponent> CombatComponent;

private:
	void OnFightingStanceTriggered();
	void OnJabStarted();
};
