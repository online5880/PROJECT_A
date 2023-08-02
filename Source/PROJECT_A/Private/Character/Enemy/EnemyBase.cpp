// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyBase.h"

#include "UtilityFunction.h"
#include "Character/Component/CombatComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::Damaged(const float Damage)
{
	if(!CombatComponent->GetDamaged())
	{
		PrintEditorMessage(5.f,__FUNCTION__);
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

