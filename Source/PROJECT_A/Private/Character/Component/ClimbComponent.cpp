// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/ClimbComponent.h"

#include "MotionWarpingComponent.h"
#include "UtilityFunction.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UClimbComponent::UClimbComponent() :
FrontTraceDistance(100.f),
FrontTraceRadius(10.f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
	if(Owner)
	{
		AnimInstance = Cast<UAnimInstance>(Owner->GetMesh()->GetAnimInstance());
		CharacterMovementComponent = Cast<UCharacterMovementComponent>(Owner->GetCharacterMovement());
		MotionWarpingComponent = Cast<UMotionWarpingComponent>(Owner->GetComponentByClass(UMotionWarpingComponent::StaticClass()));
	}
}

void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UClimbComponent::Climb()
{
	if(!bIsClimbing)
	{
		FrontHitResult = CheckFrontDistance();
		if(FrontHitResult.bBlockingHit)
		{
			HeightHitResult = CheckHeight();
			if(HeightHitResult.bBlockingHit)
			{
				const FVector CalfLocation{Owner->GetMesh()->GetSocketLocation("calf_l")};
				const FVector HitLocation{HeightHitResult.Location};

				const float Height = HitLocation.Z - CalfLocation.Z;
				ClimbType = ConvertClimbType(Height);
				PlayClimbMontage();
			}
		}
	}
}

FHitResult UClimbComponent::CheckHeight() const
{
	const FVector Location = FrontHitResult.Location+(UKismetMathLibrary::MakeRotFromX(FrontHitResult.Normal).Vector()*-50.f);
	const FVector Start = Location+FVector(0.f,0.f,200.f);
	const FVector End = Location-FVector(0.f,0.f,200.f);
	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingleForObjects(
			GetWorld(),
			Start,
			End,
			FrontTraceRadius,
			ObjectTypes,
			false,
			IgnoreActors,
			TraceType,
			HitResult,
			true);

	if(HitResult.bBlockingHit && Owner)
	{
		return HitResult;
	}
	return FHitResult();
}

FHitResult UClimbComponent::CheckFrontDistance()
{
	if(Owner)
	{
		const FVector Start = Owner->GetActorLocation();
		const FVector End = Start+Owner->GetActorForwardVector()*FrontTraceDistance;
		FHitResult HitResult;

		UKismetSystemLibrary::SphereTraceSingleForObjects(
			GetWorld(),
			Start,
			End,
			FrontTraceRadius,
			ObjectTypes,
			false,
			IgnoreActors,
			TraceType,
			HitResult,
			true);
		if(HitResult.bBlockingHit)
		{
			ClimbNormal = HitResult.Normal;
			return HitResult;
		}
	}
	return FHitResult();
}


void UClimbComponent::PrintClimbType() const
{
	if(TraceType != EDrawDebugTrace::None)
	{
		const FString TypeString = FString("Climb Type : ") + *GetEnumDisplayNameToString(ClimbType);
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,TypeString);
	}
}

void UClimbComponent::PlayClimbMontage()
{
	const bool bCanPlayMontage = (ClimbMontage != nullptr
		&& Owner != nullptr
		&& AnimInstance != nullptr
		&& CharacterMovementComponent != nullptr
		&& MotionWarpingComponent != nullptr);
	
	if(bCanPlayMontage)
	{
		PrintClimbType();

		const FName WarpName = FName("Climb");
		const FVector Location = HeightHitResult.Location;
		const FRotator Rotator = FRotator(0.f,UKismetMathLibrary::MakeRotFromX(ClimbNormal).Yaw-180.f,0.f);
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(WarpName,Location,Rotator);
		CharacterMovementComponent->SetMovementMode(MOVE_Flying);
		
		AnimInstance->Montage_Play(ClimbMontage);
		AnimInstance->OnMontageEnded.AddDynamic(this,&UClimbComponent::ClimbEnded);
		
		switch (ClimbType)
		{
		case EClimbType::ECT_Ground:
			UE_LOG(LogTemp,Warning,TEXT("PlayClimbMontage - Ground"))
			break;
		case EClimbType::ECT_1m:
			AnimInstance->Montage_JumpToSection(FName("Climb_1m"));
			break;
		case EClimbType::ECT_2m:
			AnimInstance->Montage_JumpToSection(FName("Climb_2m"));
			break;
		case EClimbType::ECT_Default: break;
		default: ;
		}

		bIsClimbing = true;
	}
}

EClimbType UClimbComponent::ConvertClimbType(const float& Height)
{
	if(Height <= 100.f)
	{
		return EClimbType::ECT_1m;
	}
	else if(Height >= 101.f)
	{
		return EClimbType::ECT_2m;
	}
	return EClimbType::ECT_Ground;
}

void UClimbComponent::ClimbEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CharacterMovementComponent->SetMovementMode(MOVE_Walking);;
	bIsClimbing = false;
}

