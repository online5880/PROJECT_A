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

	// 공격
	void Attack();
protected:
	// 파이터 공격
	void PlayFighterMontage();
	// 콤보 초기화
	void ResetCombo();

	// Climb Montage Ended Delegate Event
	UFUNCTION()
	void AttackEnded(class UAnimMontage* Montage, bool bInterrupted);

	// NotifyBegin Delegate
	UFUNCTION()
	void HandleMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	// 공격 가능한지?
	/**
	 * @brief 
	 * @return Montage 가 재생중이지 않을 때 True
	 */
	bool CanExecuteAttack() const;

private:
	// 컴포넌트 소유 액터
	UPROPERTY()
	TObjectPtr<ACharacter> Owner;

	// 애님인스턴스
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

	// 데미지를 받을 수 있는지
	UPROPERTY()
	bool bCanDamaged;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bDebug;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Debug", meta = (AllowPrivateAccess = "true"))
	float DebugTime = 5.f;

	// TODO 추후 리팩토링
	// Montage(이름, Asset) 및 MaxCombo 횟수
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TArray<FCombatMontageInfo> MontageInfos;

	// 파이터 몽타주
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FighterMontage;

	// 현재 재생중인 Montage
	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrentPlayingMontage;

	// Montage Ended Delegate 
	FOnMontageEnded OnMontageEnded;

	// 콤보 횟수
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	int32 ComboCount;

	// 최대 콤보 횟수
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	int32 MaxComboCount;

	// 체력
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float Health;

	// 최대 체력
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Stat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

public:
	FORCEINLINE bool GetDamaged() const {return bCanDamaged;}
	FORCEINLINE void SetDamaged(const bool CanDamaged) {bCanDamaged = CanDamaged;}
	
	FORCEINLINE bool GetDebug() const {return bDebug;}
	FORCEINLINE float GetDebugTime() const {return DebugTime;}
};
