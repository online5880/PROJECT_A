// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ROGANAnimInstance.generated.h"
class AROGAN;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class PROJECT_A_API UROGANAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UROGANAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	// 캐릭터 클래스
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	TObjectPtr<AROGAN> ROGANCharacter;

	// 무브먼트 컴포넌트
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	// 속도
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	double Speed;

	// 방향
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	double Direction;

	// 움직이고 있는지
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	bool bIsMove;

	// 떨어지고 있는지
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	bool bIsFalling;

	// Right Input Value
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	double RightInputValue;

	// Forward Input Value
	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	double ForwardInputValue;

	UPROPERTY(BlueprintReadOnly,Category = "Character", meta =(AllowPrivateAccess = "true"))
	bool bIsCrouching;
	
};
