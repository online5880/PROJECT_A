// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyAttributeWidget.generated.h"

class AEnemyBase;
class UProgressBar;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class PROJECT_A_API UEnemyAttributeWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere,meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox;

	UPROPERTY(EditAnywhere,meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(EditAnywhere,meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(BlueprintReadOnly,Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AEnemyBase> EnemyBase;

public:
	FORCEINLINE void SetEnemyBase(AEnemyBase* Enemy) {EnemyBase = Enemy;}
};
