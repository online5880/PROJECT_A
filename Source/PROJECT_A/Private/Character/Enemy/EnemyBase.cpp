// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/EnemyBase.h"

#include "GlobalUtilty.h"
#include "Character/Component/AttributeComponent.h"
#include "Character/Component/CombatComponent.h"
#include "Character/Enum/HitDirection.h"
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

void AEnemyBase::TakeDamage(const float Damage, const FVector& Normal, FHitResult const& HitInfo,
	const float AttackPower, AActor* DamageCauser)
{
	if(!CombatComponent->GetCanDamaged())
	{
		CombatComponent->SetCanDamaged(true);
		LaunchCharacter( (Normal*FVector(1.f,1.f,0.f))*-AttackPower,false,true);
		FTimerHandle T;
		GetWorldTimerManager().SetTimer(T,[&,Damage]
		{
			AttributeComponent->DecreaseHealth(Damage);
		},0.05f,false);
		
		const FVector Forward = GetActorForwardVector();
		const FVector ImpactLowered(HitInfo.ImpactPoint.X,HitInfo.ImpactPoint.Y,GetActorLocation().Z);
		const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

		// Forward * ToHit = |Forward||ToHit| * cos(theta) -> 내적
		const double CosTheta = FVector::DotProduct(Forward,ToHit);
		double Theta = FMath::Acos(CosTheta);
		Theta = FMath::RadiansToDegrees(Theta);
		const FVector CrossProduct = FVector::CrossProduct(Forward,ToHit);
		if(CrossProduct.Z < 0)
		{
			Theta*=-1.f;
		}
		EHitDirection HitDirection;
		FName MontageSection;
		FString KnockDown = "KnockDown_";
	
		if(Theta >= -45.f && Theta < 45.f)
		{
			HitDirection = EHitDirection::EHD_Front;
			MontageSection = *GetEnumDisplayNameToString(HitDirection);
			if(Damage >= 50.f)
			{
				MontageSection = *FString(KnockDown+GetEnumDisplayNameToString(HitDirection));
			}
		}
		else if(Theta >= -135.f && Theta < -45.f)
		{
			HitDirection = EHitDirection::EHD_Left;
			MontageSection = *GetEnumDisplayNameToString(HitDirection);
		}
		else if(Theta >= 45.f && Theta < 135.f)
		{
			HitDirection = EHitDirection::EHD_Right;
			MontageSection = *GetEnumDisplayNameToString(HitDirection);
		}
		else
		{
			HitDirection = EHitDirection::EHD_Back;
			MontageSection = *GetEnumDisplayNameToString(HitDirection);
			if(Damage >= 50.f)
			{
				MontageSection = *FString(KnockDown+GetEnumDisplayNameToString(HitDirection));
			}
		}
		
		PrintEditorMessage(3.f,MontageSection.ToString());
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance && CombatComponent->GetReactMontage())
		{
			AnimInstance->Montage_Play(CombatComponent->GetReactMontage());
			AnimInstance->Montage_JumpToSection(MontageSection);
		}
	}
}

void AEnemyBase::EndDamaged()
{
	if(CombatComponent && CombatComponent->GetCanDamaged())
	{
		CombatComponent->SetCanDamaged(false);
	}
}

