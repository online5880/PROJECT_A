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

void AEnemyBase::Init()
{
	if(AnimInstance == nullptr)
	{
		AnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	}

	UEnemyAttributeWidget* EnemyAttributeWidget = Cast<UEnemyAttributeWidget>(AttributeWidget->GetWidget());
	if(EnemyAttributeWidget)
	{
		EnemyAttributeWidget->SetEnemyBase(this);
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::CalculateHitDegree(FHitResult const& HitInfo, double& Degree) const
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(HitInfo.ImpactPoint.X,HitInfo.ImpactPoint.Y,GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
		
	const double CosTheta = FVector::DotProduct(Forward,ToHit);
	Degree = FMath::Acos(CosTheta);
	Degree = FMath::RadiansToDegrees(Degree);
	const FVector CrossProduct = FVector::CrossProduct(Forward,ToHit);
	if(CrossProduct.Z < 0)
	{
		Degree*=-1.f;
	}
}

EHitDirection AEnemyBase::GetHitDirection(const double& Degree) const
{
	EHitDirection HitDirection;

	if(Degree >= -45.f && Degree < 45.f)
	{
		HitDirection = EHitDirection::EHD_Front;
	}
	else if(Degree >= -135.f && Degree < -45.f)
	{
		HitDirection = EHitDirection::EHD_Left;
	}
	else if(Degree >= 45.f && Degree < 135.f)
	{
		HitDirection = EHitDirection::EHD_Right;
	}
	else
	{
		HitDirection = EHitDirection::EHD_Back;
	}
	return HitDirection;
}

void AEnemyBase::TakeDamage(const float Damage, const FVector& Normal, FHitResult const& HitInfo,
                            const float AttackPower, AActor* DamageCauser, TSubclassOf<UCameraShakeBase> CameraShakeBase)
{
	if(!CombatComponent->GetCanDamaged())
	{
		CombatComponent->SetCanDamaged(true);

		const FVector LaunchDirection = Normal * FVector(1.f, 1.f, 0.f) * -AttackPower;
		LaunchCharacter(LaunchDirection, false, true);
    
		FTimerHandle TakeDamageTimer;
		GetWorldTimerManager().SetTimer(TakeDamageTimer, [&, Damage]
		{
			AttributeComponent->DecreaseHealth(Damage);

			double Degree;
			CalculateHitDegree(HitInfo, Degree);
			const EHitDirection HitDirection = GetHitDirection(Degree);
			FName MontageSection = *GetEnumDisplayNameToString(HitDirection);
			const FString KnockDown = "KnockDown_";

			if (Damage >= 50.f && (HitDirection == EHitDirection::EHD_Front || HitDirection == EHitDirection::EHD_Back))
			{
				MontageSection = *FString(KnockDown + GetEnumDisplayNameToString(HitDirection));
			}

			PrintEditorMessage(3.f, MontageSection.ToString());

			PlayReactMontage(MontageSection);
		}, 0.05f, false);
		
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.1f);
		FTimerHandle SlowTimer;
		GetWorldTimerManager().SetTimer(SlowTimer,[&]
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1.f);
		},0.005f,false);

		APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
		if(PlayerCameraManager && CameraShakeBase)
		{
			PlayerCameraManager->StartCameraShake(CameraShakeBase);
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

void AEnemyBase::PlayReactMontage(const FName& Section) const
{
	if(AnimInstance && CombatComponent->GetReactMontage())
	{
		AnimInstance->Montage_Play(CombatComponent->GetReactMontage());
		AnimInstance->Montage_JumpToSection(Section);
	}
}

