#include "Character/Component/CombatComponent.h"

#include "GameFramework/Character.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::Init()
{
	Owner = Cast<ACharacter>(GetOwner());
	if (Owner)
	{
		const USkeletalMeshComponent* CharacterMesh = Owner->GetMesh();
		AnimInstance = (CharacterMesh) ? Cast<UAnimInstance>(CharacterMesh->GetAnimInstance()) : nullptr;
		if (AnimInstance)
		{
			// Montage Ended 와 NotifyBegin 에 이벤드 바인딩
			AnimInstance->OnMontageEnded.AddDynamic(this, &UCombatComponent::AttackEnded);
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UCombatComponent::HandleMontageNotifyBegin);
		}
	}
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::Attack(const FString& InfoName)
{
	// TODO Fighter 는 임시
	const FCombatMontageInfo CurrentMontageInfo = GetMontageInfo(InfoName);
	// Max ComboCount 까지 증가
	ComboCount = FMath::Clamp(++ComboCount, 1, CurrentMontageInfo.MaxComboCount);
	if(CanExecuteAttack())
	{
		PlayMontage(CurrentMontageInfo);
	}
}

void UCombatComponent::PlayMontage(const FCombatMontageInfo& Info)
{
	if(Info.Montage && AnimInstance)
	{
		AnimInstance->Montage_Play(Info.Montage);
		CurrentPlayingMontage = AnimInstance->GetCurrentActiveMontage();
	}
}

void UCombatComponent::ResetCombo()
{
	CurrentPlayingMontage = nullptr;
	ComboCount = 0;
}

void UCombatComponent::AttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ResetCombo();
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

FCombatMontageInfo UCombatComponent::GetMontageInfo(const FString& Name)
{
	FCombatMontageInfo Ret{};
	if (!MontageInfos.IsEmpty())
	{
		Ret = MontageInfos.FindRef(Name);
	}
	return Ret;
}


