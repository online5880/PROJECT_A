// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MontageStop.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_A_API UAnimNotify_MontageStop : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_MontageStop();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	// Blend 되면서 멈추는 시간
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), DisplayName = "Value")
	double BlendOutValue;

	// 디버그용 bool
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), DisplayName = "Enable")
	bool bEnable;

	virtual FString GetNotifyName_Implementation() const override;
};
