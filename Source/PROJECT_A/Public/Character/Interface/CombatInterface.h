// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * NotifyState 에서 NotifyTick 에서 데미지를 1번만 받고 NotifyEnd 가 되기 전까지는 데미지를 받지 않기 위해서 사용
 */
class PROJECT_A_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * @brief 데미지를 받음
	 * @param Damage 데미지
	 * @param Normal Normal Vector
	 * @param AttackPower 밀리는 수치
	 */
	virtual void Damaged(const float Damage, const FVector& Normal,const float AttackPower) = 0;

	/**
	 * @brief 데미지 받는게 끝남
	 */
	virtual void EndDamaged() = 0;
};
