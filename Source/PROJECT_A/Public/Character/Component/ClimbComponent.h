// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ClimbComponent.generated.h"
class UCharacterMovementComponent;
class UMotionWarpingComponent;

UENUM(BlueprintType)
enum class EClimbType : uint8
{
	ECT_Ground UMETA(DisplayName = "Ground"),
	ECT_1m UMETA(DisplayName = "1m"),
	ECT_2m UMETA(DisplayName = "2m"),

	ECT_Default UMETA(DisplayName = "DefaultMAX"),
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_A_API UClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UClimbComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Climb();
protected:
    // 높이 체크
	FHitResult CheckHeight() const;
	
	// 정면 거리 체크
	FHitResult CheckFrontDistance();

	// Climb Type 디버깅 메시지
	void PrintClimbType() const;

	// Climb Montage 플레이
	void PlayClimbMontage();

	// 높이 -> ClimbType 변환
	EClimbType ConvertClimbType(const float& Height);

	// Climb Montage Ended Delegate Event
	UFUNCTION()
	void ClimbEnded(class UAnimMontage* Montage, bool bInterrupted);

private:
#pragma region Climb

	// 컴포넌트 소유 액터
	UPROPERTY()
	TObjectPtr<ACharacter> Owner;

	// 애님인스턴스
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

	// 캐릭터 무브먼트 컴포넌트
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	// 모션 워핑 컴포넌트
	UPROPERTY()
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
	
	// Climb Montage
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ClimbMontage;

	// Climb Montage Ended Delegate 
	FOnMontageEnded OnMontageEnded;

    // 정면 체크 트레이스 거리
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	double FrontTraceDistance;

    // 정면 체크 트레이스 반경
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	double FrontTraceRadius; 

	// 오브젝트 타입
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	// 무시할 액터
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> IgnoreActors;
	
	// 트레이스 타입
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EDrawDebugTrace::Type> TraceType;

	// ClimbType (1m, 2m)
	EClimbType ClimbType;

	// 정면 트레이스 HitResult
	FHitResult FrontHitResult;

	// 높이 트레이스 HitResult
	FHitResult HeightHitResult;

	// 정면 트레이스 노멀
	FVector ClimbNormal;

	// Climb 가능한지?
	bool bIsClimbing;
	
#pragma endregion Climb
};
