// Fill out your copyright notice in the Description page of Project Settings.

#include "BEAnimNotifyState_JabWindow.h"
#include "BECharacter.h"
#include "BECombatComponent.h"
#include "Components/SkeletalMeshComponent.h"

UBEAnimNotifyState_JabWindow::UBEAnimNotifyState_JabWindow()
{
	FistSocketName = TEXT("fist_l");
}

void UBEAnimNotifyState_JabWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	TickLogCount = 0;

	UE_LOG(LogTemp, Log, TEXT("Jab Window BEGIN"));

	if (!MeshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("  MeshComp is null"));
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	UE_LOG(LogTemp, Log, TEXT("  Owner: %s"), Owner ? *Owner->GetName() : TEXT("null"));
	UE_LOG(LogTemp, Log, TEXT("  Casts to ABECharacter: %s"), Cast<ABECharacter>(Owner) ? TEXT("YES") : TEXT("NO"));
	UE_LOG(LogTemp, Log, TEXT("  Socket '%s' exists: %s"), *FistSocketName.ToString(), MeshComp->DoesSocketExist(FistSocketName) ? TEXT("YES") : TEXT("NO"));

	ABECharacter* Character = Cast<ABECharacter>(Owner);
	if (Character)
	{
		Character->GetCombatComponent()->BeginFistSweep(MeshComp, FistSocketName);
	}
}

void UBEAnimNotifyState_JabWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (TickLogCount == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Jab Window TICK (first frame)"));
	}
	++TickLogCount;

	if (!MeshComp) return;

	ABECharacter* Character = Cast<ABECharacter>(MeshComp->GetOwner());
	if (!Character) return;

	Character->GetCombatComponent()->UpdateFistSweep(MeshComp, FistSocketName);
}

void UBEAnimNotifyState_JabWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UE_LOG(LogTemp, Log, TEXT("Jab Window END (ticked %d frames)"), TickLogCount);

	if (!MeshComp) return;

	ABECharacter* Character = Cast<ABECharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->GetCombatComponent()->EndFistSweep();
	}
}

FString UBEAnimNotifyState_JabWindow::GetNotifyName_Implementation() const
{
	return TEXT("Jab Window");
}
