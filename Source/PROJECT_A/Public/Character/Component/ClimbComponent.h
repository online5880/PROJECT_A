// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	void CheckHeight();
	

private:
#pragma region Climb
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Climb_Montage;
	
	bool bIsClimbing;
	
	bool bCanClimbing;

	bool bIsWallThick;
#pragma endregion Climb
};
