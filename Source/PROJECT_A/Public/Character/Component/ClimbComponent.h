// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ClimbComponent.generated.h"


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
	void CheckHeight(FHitResult& HitResult) const;
	FHitResult CheckFrontDistance();
	

private:
#pragma region Climb

	UPROPERTY()
	AActor* Owner;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Climb_Montage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	double FrontTraceDistance;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	double FrontTraceRadius; 

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Trace", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EDrawDebugTrace::Type> TraceType;

	FVector ClimbNormal;
	
	bool bIsClimbing;
	
	bool bCanClimbing;

	bool bIsWallThick;
#pragma endregion Climb
};
