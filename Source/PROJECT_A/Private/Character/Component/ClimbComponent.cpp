// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/ClimbComponent.h"
UClimbComponent::UClimbComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UClimbComponent::Climb()
{
	CheckHeight();
}

void UClimbComponent::CheckHeight()
{
	
}

