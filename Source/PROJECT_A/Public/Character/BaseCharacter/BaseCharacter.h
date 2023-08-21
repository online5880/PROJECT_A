// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UAttributeComponent;
class AROGAN;
class AEnemyBase;
class UStatusComponent;
class UCombatComponent;
class UClimbComponent;
class UMotionWarpingComponent;

UCLASS()
class PROJECT_A_API ABaseCharacter : public ACharacter,  public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Interface
	virtual void TakeDamage(const float Damage, const FVector& Normal, FHitResult const& HitInfo, const float PushValue, AActor* DamageCauser, TSubclassOf<UCameraShakeBase> CameraShakeBase = nullptr) override{}
	virtual void EndDamaged() override{}
	virtual void MoveToTarget(AActor* Target) override{}

	void PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeBase);

#pragma endregion Component
	// 모션 워핑 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	// 벽 오르기 컴포넌트(1m, 2m)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UClimbComponent> ClimbComponent;

	// 컴뱃 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeComponent> AttributeComponent;
#pragma endregion Component

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

public:
	FORCEINLINE TObjectPtr<UMotionWarpingComponent> GetMotionWarpingComponent() const {return MotionWarpingComponent;}
	FORCEINLINE TObjectPtr<UClimbComponent> GetClimbComponent() const {return ClimbComponent;}
	FORCEINLINE TObjectPtr<UCombatComponent> GetCombatComponent() const {return CombatComponent;}
	FORCEINLINE TObjectPtr<UAttributeComponent> GetAttributeComponent() const {return AttributeComponent;}
};
