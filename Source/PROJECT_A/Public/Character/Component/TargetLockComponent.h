// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetLockComponent.generated.h"
class UNiagaraComponent;
class UNiagaraSystem;
DECLARE_DELEGATE(FDisableTargetLockDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_A_API UTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UTargetLockComponent();

	void TargetLock();

	FDisableTargetLockDelegate DisableTargetLockDelegate;
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void BeginPlay() override;
	
	/**
	 * @brief 초기화
	 */
	void Init();

	/**
	 * @brief 카메라 회전
	 * @param DeltaTime DeltaTime
	 */
	void RotateCamera(float DeltaTime) const;
	void DisableTargetLock();

	void SearchTarget();
private:
	/**
	 * @brief Target Lock 이 걸렸는지
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= "Lock", meta = (AllowPrivateAccess = "true"))
	bool bIsTargetLock = false;

	/**
	 * @brief 탐색 거리
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Lock", meta = (AllowPrivateAccess = "true"))
	float SearchDistance = 500.f;

	/**
	 * @brief 탐색 범위
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Lock", meta = (AllowPrivateAccess = "true"))
	float SearchSphereRadius = 125.f;

	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> TargetLockEffect;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> Effect;
};
