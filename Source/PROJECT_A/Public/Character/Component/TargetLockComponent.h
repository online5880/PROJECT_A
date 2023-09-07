// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetLockComponent.generated.h"
class UNiagaraComponent;
class UNiagaraSystem;
DECLARE_DELEGATE(FDisableTargetLockDelegate);
DECLARE_DELEGATE(FOnTargetLock);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_A_API UTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UTargetLockComponent();
	
	FDisableTargetLockDelegate DisableTargetLockDelegate;

	FOnTargetLock OnTargetLock;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void BeginPlay() override;

	/**
	* @brief 초기화
	*/
	void Init();

	/**
	 * @brief 이펙트 위치
	 */
	void SetEffectLocation() const;
	
	/**
	 * @brief 이펙트 활성화
	 */
	void ActivateEffect();

	/**
	 * @brief 타겟락
	 */
	void TargetLock();
	
	/**
	 * @brief 카메라 회전
	 * @param DeltaTime DeltaTime
	 */
	void RotateCamera(float DeltaTime) const;

	/**
	 * @brief 타겟 락 비활성화
	 */
	UFUNCTION()
	void DisableTargetLock();

	/**
	 * @brief 타겟 탐색
	 */
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


	/**
	 * @brief 컴포넌트 오너 액터
	 */
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

	/**
	 * @brief 타겟 액터
	 */
	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	/**
	 * @brief 타겟 락 이펙트(나이아가라)
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effect",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> TargetLockEffect;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> Effect;

public:
	FORCEINLINE bool GetIsTargetLock() const {return bIsTargetLock;}
};
