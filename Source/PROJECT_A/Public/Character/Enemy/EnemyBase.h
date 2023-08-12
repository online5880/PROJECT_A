// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter/BaseCharacter.h"
#include "EnemyBase.generated.h"

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

	// Combat Interface
	virtual void Damaged(const float Damage,const FVector& Normal,const float AttackPower) override;
	virtual void EndDamaged() override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> AttributeWidget;

	UPROPERTY()
	TSubclassOf<UUserWidget> AttributeWidgetClass;
};
