// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/ClimbComponent.h"

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

	Owner = GetOwner();
}

void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UClimbComponent::Climb()
{
	FHitResult HitResult = CheckFrontDistance();
	if(HitResult.bBlockingHit)
	{
		CheckHeight(HitResult);
	}
}

void UClimbComponent::CheckHeight(FHitResult& HitResult) const
{
	const FVector Location = HitResult.Location+(UKismetMathLibrary::MakeRotFromX(HitResult.Normal).Vector()*-50.f);
	const FVector Start = Location+FVector(0.f,0.f,200.f);
	const FVector End = Location-FVector(0.f,0.f,200.f);
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

