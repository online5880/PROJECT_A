// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/TargetLockComponent.h"

#include "GlobalUtilty.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UTargetLockComponent::UTargetLockComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTargetLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotateCamera(DeltaTime);

	SetEffectLocation();
}

void UTargetLockComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UTargetLockComponent::Init()
{
	if(OwnerActor == nullptr)
	{
		OwnerActor = GetOwner();
	}
	
	ActivateEffect();

	DisableTargetLockDelegate.BindUObject(this,&UTargetLockComponent::DisableTargetLock);
	OnTargetLock.BindUObject(this,&UTargetLockComponent::TargetLock);
}

void UTargetLockComponent::SetEffectLocation() const
{
	if(TargetActor && OwnerActor && bIsTargetLock && Effect)
	{
		const FVector EffectLocation = TargetActor->GetRootComponent()->GetComponentLocation()
			+FVector(0.f,0.f,TargetActor->GetRootComponent()->GetComponentLocation().Z*1.1f);
		
		Effect->SetWorldLocation(EffectLocation);
	}
}

void UTargetLockComponent::ActivateEffect()
{
	if(TargetLockEffect)
	{
		Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			OwnerActor->GetWorld(),
			TargetLockEffect,
			FVector::ZeroVector);
		
		if(Effect)
		{
			Effect->SetVisibility(false);
		}
	}
}

void UTargetLockComponent::TargetLock()
{
	const bool bCanTargetLock = bIsTargetLock && TargetActor && Effect;
	if(bCanTargetLock)
	{
		DisableTargetLock();
		return;
	}
	SearchTarget();
}

void UTargetLockComponent::RotateCamera(float DeltaTime) const
{
	if(TargetActor && OwnerActor && bIsTargetLock)
	{
		AController* Controller = OwnerActor->GetInstigatorController();
		if(Controller)
		{
			const FRotator ControllerRot = Controller->GetControlRotation();
			const FVector Start = OwnerActor->GetActorLocation();
			const FVector End = TargetActor->GetActorLocation();
			const FRotator FindLookRot = UKismetMathLibrary::FindLookAtRotation(Start,End);
			const FRotator TargetRot = FRotator(ControllerRot.Pitch,FindLookRot.Yaw,ControllerRot.Roll);
			const FRotator RInterpTo = UKismetMathLibrary::RInterpTo_Constant(ControllerRot,TargetRot,DeltaTime,100.f);
			
			Controller->SetControlRotation(TargetRot);
		}
	}
}


void UTargetLockComponent::DisableTargetLock()
{
	if(Effect)
	{
		Effect->SetVisibility(false);
	}
	if(TargetActor)
	{
		TargetActor = nullptr;
	}
	bIsTargetLock = false;
}

void UTargetLockComponent::SearchTarget()
{
	if(OwnerActor)
	{
		const UWorld* World = OwnerActor->GetWorld();

		const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(OwnerActor->GetWorld(),0);
		if(CameraManager)
		{
			FVector ForwardVector = CameraManager->GetCameraRotation().Vector();
			FVector Start = OwnerActor->GetActorLocation()+OwnerActor->GetActorForwardVector()*SearchSphereRadius;
			FVector End = Start+(ForwardVector*SearchDistance);

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
			ObjectType.Add(EnemyObjectType);
			TArray<AActor*> IgnoreActors;
			IgnoreActors.Emplace(OwnerActor);
			FHitResult HitResult;
			
			UKismetSystemLibrary::SphereTraceSingleForObjects(
			World,
			Start,
			End,
			SearchSphereRadius,
			ObjectType,
			false,
			IgnoreActors,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.f);

			if(HitResult.bBlockingHit)
			{
				AActor* Target = HitResult.GetActor();
				if(Target && Effect)
				{
					TargetActor = Target;
					bIsTargetLock = true;
					Effect->SetVisibility(true);
				}
			}
		}
	}
}


