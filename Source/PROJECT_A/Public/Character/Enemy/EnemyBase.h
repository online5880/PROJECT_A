// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class PROJECT_A_API AEnemyBase : public ACharacter, public ICombatInterface
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
	virtual void Damaged(const float Damage) override;;
	virtual void EndDamaged() override;

private:
	// 컴뱃 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCombatComponent> CombatComponent;

};
