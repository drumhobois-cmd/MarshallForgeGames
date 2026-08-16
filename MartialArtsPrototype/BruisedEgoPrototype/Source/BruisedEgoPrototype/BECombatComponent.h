// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	// Fist sweep window — called by UBEAnimNotifyState_JabWindow
	void BeginFistSweep(USkeletalMeshComponent* Mesh, FName SocketName);
	void UpdateFistSweep(USkeletalMeshComponent* Mesh, FName SocketName);
	void EndFistSweep();

private:
	UPROPERTY(VisibleAnywhere, Category="Combat")
	bool bIsInFightingStance;

	bool bFistSweepActive = false;
	FVector PreviousFistLocation = FVector::ZeroVector;
	bool bHitLoggedThisWindow = false;
};
