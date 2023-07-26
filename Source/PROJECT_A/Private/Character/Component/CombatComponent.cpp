#include "Character/Component/CombatComponent.h"

#include "GameFramework/Character.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
	if(Owner)
	{
		AnimInstance = Cast<UAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::Attack()
{
	UE_LOG(LogTemp,Warning,TEXT("Attack"));
	if(ComboCount > MaxComboCount)
	{
		ComboCount++;
		Attack_Fighter();
	}
}

void UCombatComponent::Attack_Fighter()
{
	if(FighterMontage && AnimInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,__FUNCTION__);
		AnimInstance->Montage_Play(FighterMontage);
	}
}


