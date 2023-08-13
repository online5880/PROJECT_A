// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/AttributeComponent.h"

#include "..\..\..\Public\GlobalUtilty.h"
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
	if(OwnerCharacter != nullptr)
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

void UAttributeComponent::DecreaseHealth(const float Value)
{
	if(Health > 0.f)
	{
		Health-=Value;
		
		if(Health <= 0)
		{
			Health = 0.f;
			if(OwnerCharacter)
			{
				UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(OwnerCharacter->GetComponentByClass(UWidgetComponent::StaticClass()));
				OwnerCharacter->GetMesh()->SetCollisionProfileName("Ragdoll");
				OwnerCharacter->GetMesh()->SetSimulatePhysics(true);
				OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				if(WidgetComponent)
				{
					WidgetComponent->SetVisibility(false);
				}
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

