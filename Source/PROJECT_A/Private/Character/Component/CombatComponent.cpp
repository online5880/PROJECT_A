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
	if (Owner)
	{
		const USkeletalMeshComponent* CharacterMesh = Owner->GetMesh();
		AnimInstance = (CharacterMesh) ? Cast<UAnimInstance>(CharacterMesh->GetAnimInstance()) : nullptr;
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &UCombatComponent::AttackEnded);
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UCombatComponent::HandleMontageNotifyBegin);
		}
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::Attack()
{
	ComboCount = FMath::Clamp(++ComboCount, 1, MaxComboCount);
	if(CanExecuteAttack())
	{
		PlayFighterMontage();
	}
}

void UCombatComponent::PlayFighterMontage()
{
	if(FighterMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(FighterMontage);
		CurrentPlayingMontage = AnimInstance->GetCurrentActiveMontage();
	}
}

void UCombatComponent::AttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CurrentPlayingMontage = nullptr;
	ComboCount = 0;
}

void UCombatComponent::HandleMontageNotifyBegin(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if(CurrentPlayingMontage && ComboCount > 0)
	{
		ComboCount--;
		if(ComboCount == 0)
		{
			AnimInstance->Montage_Stop(0.25f,CurrentPlayingMontage);
		}
	}
}

bool UCombatComponent::CanExecuteAttack() const
{
	if(AnimInstance)
	{
		return !AnimInstance->IsAnyMontagePlaying();
	}
	return false;
}


