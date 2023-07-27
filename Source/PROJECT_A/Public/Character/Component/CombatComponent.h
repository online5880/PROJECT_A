// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FCombatMontageInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxComboCount;
};

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

	// Climb Montage Ended Delegate Event
	UFUNCTION()
	void AttackEnded(class UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void HandleMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

private:
	// 컴포넌트 소유 액터
	UPROPERTY()
	TObjectPtr<ACharacter> Owner;

	// 애님인스턴스
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

	// TODO 추후 리팩토링
	// Montage(이름, Asset) 및 MaxCombo 횟수
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TArray<FCombatMontageInfo> MontageInfos;

	// 파이터 몽타주
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FighterMontage;

	// 현재 재생중인 Montage
	UPROPERTY();
	TObjectPtr<UAnimMontage> CurrentPlayingMontage;

	// Montage Ended Delegate 
	FOnMontageEnded OnMontageEnded;

	// 콤보 횟수
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	int32 ComboCount;

	// 최대 콤보 횟수
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	int32 MaxComboCount;
};
