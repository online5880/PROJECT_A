// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Animation/ROGANAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/ROGAN.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UROGANAnimInstance::UROGANAnimInstance()
{
}

void UROGANAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if(ROGANCharacter == nullptr)
	{
		ROGANCharacter = Cast<AROGAN>(GetOwningActor());
		if(ROGANCharacter)
		{
			MovementComponent = Cast<UCharacterMovementComponent>(ROGANCharacter->GetMovementComponent());
		}
	}
}

void UROGANAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(ROGANCharacter && MovementComponent)
	{
		Speed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		Direction = UKismetAnimationLibrary::CalculateDirection(MovementComponent->Velocity,ROGANCharacter->GetActorRotation()); 
		bIsMove = ((Speed > 3.f) && (MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector));
		bIsFalling = ROGANCharacter->GetIsFalling();
		bIsCrouching = ROGANCharacter->GetIsCrouch();
		bIsDodge = ROGANCharacter->GetIsDodge();
		ForwardInputValue = ROGANCharacter->GetForwardInputValue();
		RightInputValue = ROGANCharacter->GetRightInputValue();
	}
}