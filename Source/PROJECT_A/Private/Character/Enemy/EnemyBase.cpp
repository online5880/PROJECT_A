// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyBase.h"
#include "Character/Component/AttributeComponent.h"
#include "Character/Component/CombatComponent.h"
#include "Character/Widget/EnemyAttributeWidget.h"
#include "Components/WidgetComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	/*static ConstructorHelpers::FClassFinder<UUserWidget>
	AttributeWidgetBP(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/_Game/Character/Enemy/Widget/WBP_EnemyAttribute.WBP_EnemyAttribute_C'"));
	if(AttributeWidgetBP.Succeeded()) {AttributeWidgetClass = AttributeWidgetBP.Class;}*/

	AttributeWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Attribute Widget"));
	AttributeWidget->SetupAttachment(GetRootComponent());
	AttributeWidget->SetDrawAtDesiredSize(true);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	UEnemyAttributeWidget* EnemyAttributeWidget = Cast<UEnemyAttributeWidget>(AttributeWidget->GetWidget());
	if(EnemyAttributeWidget)
	{
		EnemyAttributeWidget->SetEnemyBase(this);
	}
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
		FTimerHandle T;
		GetWorldTimerManager().SetTimer(T,[&,Damage]
		{
			AttributeComponent->DecreaseHealth(Damage);
		},0.05f,false);
	}
}

void AEnemyBase::EndDamaged()
{
	if(CombatComponent && CombatComponent->GetDamaged())
	{
		CombatComponent->SetDamaged(false);
	}
}

