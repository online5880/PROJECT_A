// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/AttributeComponent.h"

#include "..\..\..\Public\GlobalUtilty.h"
#include "Character/Component/TargetLockComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"

UAttributeComponent::UAttributeComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
}

void UAttributeComponent::Init()
{
	if(OwnerCharacter == nullptr)
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());
	}
}

void UAttributeComponent::PrintCurrentHealth() const
{
	PrintEditorMessage(3.f,FString::Printf(TEXT("Current Health :  %f"),Health));
}

float UAttributeComponent::GetHealthPercentage() const
{
	return (Health/MaxHealth);
}

void UAttributeComponent::Die()
{
	if(OwnerCharacter && DamageCauserActor)
	{
		UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(OwnerCharacter->GetComponentByClass(UWidgetComponent::StaticClass()));
		const UTargetLockComponent* TargetLockComponent = Cast<UTargetLockComponent>(DamageCauserActor->GetComponentByClass(UTargetLockComponent::StaticClass()));
		OwnerCharacter->GetMesh()->SetCollisionProfileName("Ragdoll");
		OwnerCharacter->GetMesh()->SetSimulatePhysics(true);
		OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		OwnerCharacter->SetLifeSpan(5.f);
		
		TargetLockComponent->DisableTargetLockDelegate.ExecuteIfBound();
		
		if(WidgetComponent)
		{
			WidgetComponent->SetVisibility(false);
		}
		
		bIsDead = true;
	}
}

void UAttributeComponent::DecreaseHealth(const float Value, AActor* DamageCauser)
{
	DamageCauserActor = DamageCauser;
	if(Health > 0.f && !bIsDead)
	{
		Health-=Value;
		
		if(Health <= 0)
		{
			Health = 0.f;
			
			Die();
		}
	}
	else
	{
		Die();
	}
}

void UAttributeComponent::IncreaseHealth(const float Value)
{
	if(Health < MaxHealth)
	{
		Health+=Value;
		if(Health > MaxHealth) {Health = MaxHealth;}
	}
}

