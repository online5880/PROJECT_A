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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init();
	void PrintCurrentHealth() const;

private:
	// 체력
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;

	// 최대 체력
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;

	UFUNCTION(BlueprintCallable , Category = "Function")
	float GetHealthPercentage() const;

public:
	// Health Getter, Setter
	FORCEINLINE float GetHealth() const {return Health;}
	FORCEINLINE void SetHealth(const float ChangeHealth)  {Health = ChangeHealth;}
	
	// MaxHealth Getter, Setter
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}
	FORCEINLINE void SetMaxHealth(const float ChangeHealth)  {MaxHealth = ChangeHealth;}
};
