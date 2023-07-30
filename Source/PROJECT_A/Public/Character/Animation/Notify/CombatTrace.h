// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CombatTrace.generated.h"

class UCombatComponent;
/**
 * 
 */
UCLASS()
class PROJECT_A_API UCombatTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UCombatTrace();

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

	// Draw Trace 타입
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType(const USkeletalMeshComponent* MeshComp) const;

	// Debug Trace 시간
	float DebugTime(const USkeletalMeshComponent* MeshComp) const;

	// Combat Component 를 MeshComp 로 부터 캐스팅
	TObjectPtr<UCombatComponent> GetCombatComponentFromMesh(const USkeletalMeshComponent* MeshComp) const;
private:
	UPROPERTY()
	TArray<FHitResult> HitResults;

	// Start trace mesh socket name
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Trace", meta =(AllowPrivateAccess = "true"))
	FName StartSocketName;

	// End trace mesh socket name
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Trace", meta =(AllowPrivateAccess = "true"))
	FName EndSocketName;

	// 공격 범위
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Trace", meta =(AllowPrivateAccess = "true"))
	double AttackRadius;

	// 오브젝트 타입
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	// 무시할 액터
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AActor>> IgnoreActors;
};
