// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class AEnemyBase;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class PROJECT_A_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	// 캐릭터 클래스
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	TObjectPtr<AEnemyBase> Character;

	// 무브먼트 컴포넌트
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	// 속도
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	double Speed;

	// 방향
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	double Direction;
	
};