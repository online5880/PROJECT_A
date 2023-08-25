// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_MotionWarping.h"
#include "NotifyState_MeleeMotionWarping.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_A_API UNotifyState_MeleeMotionWarping : public UAnimNotifyState_MotionWarping
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

	/**
	 * @brief 근처 타겟 검색
	 * @param MeshComp MeshComp
	 * @return TArray<AActor*> 탐색 액터 배열
	 */
	TArray<AActor*> FindNearTarget(const USkeletalMeshComponent* MeshComp) const;
};
