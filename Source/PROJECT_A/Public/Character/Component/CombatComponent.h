// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_A_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Attack();
protected:
	void Attack_Fighter();

private:
	// 컴포넌트 소유 액터
	UPROPERTY()
	TObjectPtr<ACharacter> Owner;

	// 애님인스턴스
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

	// 파이터 몽타주
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FighterMontage;
};
