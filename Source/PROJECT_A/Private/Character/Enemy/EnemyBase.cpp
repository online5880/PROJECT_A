// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyBase.h"
#include "UtilityFunction.h"
#include "Character/Component/CombatComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::Damaged(const float Damage,const FVector& Normal,const float AttackPower)
{
	if(!CombatComponent->GetDamaged())
	{
		LaunchCharacter( (Normal*FVector(1.f,1.f,0.f))*-AttackPower,false,true);
		CombatComponent->SetDamaged(true);
	}
}

void AEnemyBase::EndDamaged()
{
	if(CombatComponent && CombatComponent->GetDamaged())
	{
		CombatComponent->SetDamaged(false);
	}
}

