// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter/BaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "Character/Component/ClimbComponent.h"
#include "Character/Component/CombatComponent.h"
#include "Character/Component/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	ClimbComponent = CreateDefaultSubobject<UClimbComponent>(TEXT("Climb Component"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Status Component"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeBase)
{
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
	if(PlayerCameraManager && CameraShakeBase)
	{
		PlayerCameraManager->StartCameraShake(CameraShakeBase);
	}
}
