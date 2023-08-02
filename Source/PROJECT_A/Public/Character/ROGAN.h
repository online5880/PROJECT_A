// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Enum/CharacterState.h"
#include "Interface/CombatInterface.h"
#include "ROGAN.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCombatComponent;
class UClimbComponent;
class UROGANAnimInstance;
class UMotionWarpingComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PROJECT_A_API AROGAN : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	AROGAN();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	void Init();
	void SmoothCameraRotation(float DeltaTime);
	void CheckFalling();
	
	// Movement
	void Move(const FInputActionValue& Value);
	void ResetMovementValue();
	void Walk(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void Crouching(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	UFUNCTION()
	virtual void OnJumped_Implementation() override;
	virtual void Landed(const FHitResult& Hit) override;

	// Combat
	void Attack();

	// Interface
	virtual void Damaged(const float Damage) override;
	virtual void EndDamaged() override;

private:
	
#pragma region Component
	// 스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	// 모션 워핑 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	// 벽 오르기 컴포넌트(1m, 2m)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UClimbComponent> ClimbComponent;

	// 컴뱃 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> CombatComponent;
#pragma endregion Component
	// 애님인스턴스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UROGANAnimInstance> AnimInstance;

	// 맵핑 콘텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

#pragma region InputAction
	// 점프 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	// 무브 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	// 걷기 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> WalkAction;

	// 달리기 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	// 앉기 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;

	// 회피 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DodgeAction;

	// 카메라 움직임 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	// 공격 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;
#pragma endregion InputAction

#pragma region Movement Variables
	// 움직임 값 - X
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	double RightInputValue;

	// 움직임 값 - Y
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	double ForwardInputValue;

	// 떨어지고 있는 중인지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;

	// 걷는 중인지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsWalk;

	// 달리는 중인지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;

	// 앉아 있는지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouch;

	// 회피 중인지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsDodge;
#pragma endregion Movement Variables

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE double GetForwardInputValue() const {return ForwardInputValue;} 
	FORCEINLINE double GetRightInputValue() const {return RightInputValue;}
	FORCEINLINE bool GetIsFalling() const {return bIsFalling;}
	FORCEINLINE bool GetIsCrouch() const {return bIsCrouch;}
	FORCEINLINE bool GetIsDodge() const {return bIsDodge;}
};
