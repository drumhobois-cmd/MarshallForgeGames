// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BEAnimNotifyState_JabWindow.generated.h"

UCLASS()
class BRUISEDEGOPROTOTYPE_API UBEAnimNotifyState_JabWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UBEAnimNotifyState_JabWindow();

	UPROPERTY(EditAnywhere, Category="Jab Window")
	FName FistSocketName;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

private:
	int32 TickLogCount = 0;
};
