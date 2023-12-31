// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_DELEGATE(FOnGuardDelegate);

USTRUCT(Atomic, BlueprintType)
struct FCombatMontageInfo
{
	GENERATED_BODY()
public:
	FCombatMontageInfo() :
	Montage(nullptr),
	PlayRate(1.f),
	MaxComboCount(0)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxComboCount;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_A_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	
	// 초기화
	void Init();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 공격
	 * @param InfoName TMap<FString,FCombatMontageInfo> 의 Key
	 */
	void Attack(const FString& InfoName);

	FOnGuardDelegate OnGuardDelegate;
protected:
	// 파이터 공격
	void PlayMontage(const FCombatMontageInfo& Info);
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

	FCombatMontageInfo GetMontageInfo(const FString& Name);

	void PlayGuardMontage();

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

	// 디버그 On/Off
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bDebug;

	// 디버그 타임
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Debug", meta = (AllowPrivateAccess = "true"))
	float DebugTime = 5.f;

	// TODO 추후 리팩토링
	// Montage(이름, Asset) 및 MaxCombo 횟수 배열
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TMap<FString,FCombatMontageInfo> CombatMontageInfoMap;
	
	// 현재 재생중인 Montage
	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrentPlayingMontage;

	// Montage Ended Delegate 
	FOnMontageEnded OnMontageEnded;

	// 콤보 횟수
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	int32 ComboCount;

	/*// 최대 콤보 횟수
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	int32 MaxComboCount;*/

	// 피격 Montage
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ReactMontage;

	// 가드 Montage
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> GuardMontage;
	
public:
	FORCEINLINE bool GetCanDamaged() const {return bCanDamaged;}
	FORCEINLINE void SetCanDamaged(const bool CanDamaged) {bCanDamaged = CanDamaged;}
	
	FORCEINLINE bool GetDebug() const {return bDebug;}
	FORCEINLINE float GetDebugTime() const {return DebugTime;}

	FORCEINLINE TObjectPtr<UAnimMontage> GetReactMontage() const {return ReactMontage;}
	FORCEINLINE TObjectPtr<UAnimMontage> GetGuardMontage() const {return GuardMontage;}
};
