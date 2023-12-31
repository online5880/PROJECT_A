// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/BaseCharacter/BaseCharacter.h"
#include "ROGAN.generated.h"

enum class ECharacterMovementState : uint8;
class USpringArmComponent;
class UCameraComponent;
class UROGANAnimInstance;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PROJECT_A_API AROGAN : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AROGAN();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	// 초기화
	void Init();
	// 카메라 회전(Use deltaTime)
	void SmoothCameraRotation(float DeltaTime);
	// 떨어지는지 체크
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

	// TargetLock
	virtual void TargetLock() override;

	// Interface
	virtual void TakeDamage(const float Damage, const FVector& Normal, FHitResult const& HitInfo, const float PushValue, AActor* DamageCauser,TSubclassOf<UCameraShakeBase> CameraShakeBase = nullptr) override;
	virtual void EndDamaged() override;
	virtual void MoveToTarget(AActor* Target) override;
	virtual void MoveToTargets(TArray<AActor*> Targets) override;

private:
	
#pragma region Component
	// 스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

#pragma endregion Component
	// 애님인스턴스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UROGANAnimInstance> AnimInstance;

#pragma region InputAction
	// 맵핑 콘텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
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

	// 공격 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> TargetLockAction;
#pragma endregion InputAction

#pragma region Movement Variables
	// Movement State(Enum)
	UPROPERTY()
	ECharacterMovementState MovementState;
	
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

	float SprintTime;

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
