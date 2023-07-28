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
		if(AnimInstance)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this,&UCombatComponent::AttackEnded);
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&UCombatComponent::HandleMontageNotifyBegin);
		}
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::Attack()
{
	ComboCount = 1;
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,FString::Printf(TEXT("Combo Count : %d"),ComboCount));
	if(!AnimInstance->IsAnyMontagePlaying())
	{
		Attack_Fighter();
	}
}

void UCombatComponent::Attack_Fighter()
{
	if(FighterMontage && AnimInstance)
	{
		//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,__FUNCTION__);
		AnimInstance->Montage_Play(FighterMontage);
		CurrentPlayingMontage = AnimInstance->GetCurrentActiveMontage();
	}
}

void UCombatComponent::AttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CurrentPlayingMontage = nullptr;
	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,__FUNCTION__);
}

void UCombatComponent::HandleMontageNotifyBegin(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if(CurrentPlayingMontage)
	{
		if(ComboCount == 1)
		{
			ComboCount--;
		}
		else
		{
			AnimInstance->Montage_Stop(0.25f,CurrentPlayingMontage);
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,FString::Printf(TEXT("Stop")));
		}
	}
}


