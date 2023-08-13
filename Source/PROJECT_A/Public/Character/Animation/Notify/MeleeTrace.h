// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MeleeTrace.generated.h"

class UCombatComponent;
/**
 * 
 */
UCLASS()
class PROJECT_A_API UMeleeTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UMeleeTrace();

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

	// Draw Trace 타입
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType(const USkeletalMeshComponent* MeshComp) const;

	// Debug Trace 시간
	float DebugTime(const USkeletalMeshComponent* MeshComp) const;

	// Combat Component 를 MeshComp 로 부터 캐스팅
	TObjectPtr<UCombatComponent> GetCombatComponentFromMesh(const USkeletalMeshComponent* MeshComp) const;

	// 공격 소리 재생
	void PlayAttackSound(const UWorld* World) const;

	/**
	 * @brief HitActor(CombatInterface->Damaged 실행)
	 * @param HitActorArr HitActor 배열
	 */
	void ExecuteDamagedOnHitActors(const TSet<TObjectPtr<AActor>>& HitActorArr,const FVector& Normal,const FHitResult& HitResult,AActor* DamageCauser);

	/**
	 * @brief HitActor(CombatInterface->EndDamaged 실행)
	 * @param HitActorArr HitActor 배열
	 */
	void ExecuteEndDamagedOnHitActors(const TSet<TObjectPtr<AActor>>& HitActorArr);
	
private:
	UPROPERTY()
	TArray<FHitResult> HitResults;

	// Trace 에 감지된 액터 배열
	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors;

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

	// 공격 소리 에셋
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> AttackSound;

	// 공격 소리 볼륨
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float AttackSoundVolume = 1.f;

	// 공격 파워(밀려지는 수치)
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Melee", meta = (AllowPrivateAccess = "true"))
	float AttackPower = 500.f;

	// 공격 데미지
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Melee", meta = (AllowPrivateAccess = "true"))
	float AttackDamage;

	
};
