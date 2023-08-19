// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_A_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	/**
	 * @brief 체력 감소
	 * @param Value 값
	 */
	void DecreaseHealth(const float Value);
	/**
	 * @brief 체력 증가
	 * @param Value 값
	 */
	void IncreaseHealth(const float Value);
	
protected:
	virtual void BeginPlay() override;

	/**
	 * @brief 초기화
	 */
	void Init();

	/**
	 * @brief 현재 체력 디버깅 메시지
	 */
	void PrintCurrentHealth() const;

	
	/**
	 * @brief 체력 퍼센트 구하기
	 * @return 퍼센트
	 */
	UFUNCTION(BlueprintCallable , Category = "Function")
	float GetHealthPercentage() const;

	/**
	 * @brief 죽음
	 */
	void Die();
	
private:
	/**
	 * @brief 컴포넌트 주인 캐릭터
	 */
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;
	
	// 체력
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;

	// 최대 체력
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;

	// 죽었는지
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Stat", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

public:
	// Health Getter, Setter
	FORCEINLINE float GetHealth() const {return Health;}
	FORCEINLINE void SetHealth(const float ChangeHealth)  {Health = ChangeHealth;}
	
	// MaxHealth Getter, Setter
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}
	FORCEINLINE void SetMaxHealth(const float ChangeHealth)  {MaxHealth = ChangeHealth;}
};
