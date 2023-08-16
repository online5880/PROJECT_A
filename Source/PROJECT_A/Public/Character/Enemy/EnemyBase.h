// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter/BaseCharacter.h"
#include "EnemyBase.generated.h"

enum class EHitDirection : uint8;
class UWidgetComponent;
class UUserWidget;

UCLASS()
class PROJECT_A_API AEnemyBase : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * @brief 초기화
	 */
	void Init();

	/**
	 * @brief 피격 각도 계산 (double)
	 * @param HitInfo FHitResult
	 * @param Degree HitDirection(double)
	 */
	void CalculateHitDegree(FHitResult const& HitInfo, double& Degree) const;

	/**
	 * @brief 방향 방향 구하기 (EHitDirection)
	 * @param Degree 각도
	 * @return 맞은 방향
	 */
	EHitDirection GetHitDirection(const double& Degree) const;

	// Combat Interface
	virtual void TakeDamage(const float Damage, const FVector& Normal, FHitResult const& HitInfo, const float AttackPower, AActor* DamageCauser,TSubclassOf<UCameraShakeBase> CameraShakeBase = nullptr) override;
	virtual void EndDamaged() override;
	
	void PlayReactMontage(const FName& Section) const;

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> AttributeWidget;

	UPROPERTY()
	TSubclassOf<UUserWidget> AttributeWidgetClass;

	UPROPERTY()
	UAnimInstance* AnimInstance;
};
