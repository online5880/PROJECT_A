// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Engine/DataTable.h"
#include "FootStepSound.generated.h"

class UDataTable;

USTRUCT(BlueprintType)
struct FFootStepProperty : public FTableRowBase
{
	GENERATED_BODY()
public:
	// EPhysicalSurface 타입
	UPROPERTY(EditDefaultsOnly,Category = "Sound")
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	// 재생될 소리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> Sound;
};

/**
 * 
 */
UCLASS()
class PROJECT_A_API UFootStepSound : public UAnimNotify
{
	GENERATED_BODY()

public:
	UFootStepSound();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
private:
	/**
	 * @brief EPhysicalSurface 에 맞는 SoundBase 를 쉽게 관리하기 위해서 DataTable 사용
	 */
	UPROPERTY(BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> DT_FootStepSound;
private:
};
