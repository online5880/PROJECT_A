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
 * 
 */
class PROJECT_A_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * @brief 
	 * @param Damage 데미지
	 * @param Normal Normal Vector
	 * @param AttackPower 밀리는 수치
	 */
	virtual void Damaged(const float Damage, const FVector& Normal,const float AttackPower) = 0;

	virtual void EndDamaged() = 0;
};
