// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/EnemyAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if(Character == nullptr)
	{
		Character = Cast<AEnemyBase>(GetOwningActor());
		if(Character)
		{
			MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
		}
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(Character && MovementComponent)
	{
		Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		Direction = UKismetAnimationLibrary::CalculateDirection(MovementComponent->Velocity,Character->GetActorRotation()); 
	}
}

