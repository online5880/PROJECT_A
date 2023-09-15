// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter/BaseCharacter.h"
#include "EnemyBase.generated.h"

class AMeleeEnemyAIController;
class UNiagaraComponent;
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
	 * @brief Attribute Widget Visible
	 */
	void ShowAttributeWidget();

	/**
	 * @brief 타겟이 있으면 타겟을 바라봄
	 * @param Delta DeltaTime
	 */
	void LookTarget(float Delta);
	
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
	void SetDamageCauser(AActor* DamageCauser);

	// Combat Interface
	virtual void TakeDamage(const float Damage, const FVector& Normal, FHitResult const& HitInfo, const float PushValue, AActor* DamageCauser,TSubclassOf<UCameraShakeBase> CameraShakeBase = nullptr) override;
	virtual void EndDamaged() override;

	/**
	 * @brief Play React Montage
	 * @param Section Montage Section
	 */
	void PlayReactMontage(const FName& Section) const;

	void PlayGuardMontage() const;

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> AttributeWidget;
	
	UPROPERTY()
	TSubclassOf<UUserWidget> AttributeWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

	UPROPERTY()
	TObjectPtr<AMeleeEnemyAIController> MeleeEnemyAIController;
};
