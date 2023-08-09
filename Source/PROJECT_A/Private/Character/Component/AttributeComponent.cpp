// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/AttributeComponent.h"

#include "UtilityFunction.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::Init()
{
	Health = MaxHealth;
}

void UAttributeComponent::PrintCurrentHealth() const
{
	PrintEditorMessage(3.f,FString::Printf(TEXT("Current Health :  %f"),Health));
}

void UAttributeComponent::DecreaseHealth(const float Value)
{
	if(Health > 0.f)
	{
		Health-=Value;
		if(Health <= 0)
		{
			Health = 0.f;
			ACharacter* Character = Cast<ACharacter>(GetOwner());
			if(Character)
			{
				Character->GetMesh()->SetCollisionProfileName("Ragdoll");
				Character->GetMesh()->SetSimulatePhysics(true);
				Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
		//PrintCurrentHealth();
	}
	else
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if(Character)
		{
			Character->GetMesh()->SetCollisionProfileName("Ragdoll");
			Character->GetMesh()->SetSimulatePhysics(true);
		}
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

