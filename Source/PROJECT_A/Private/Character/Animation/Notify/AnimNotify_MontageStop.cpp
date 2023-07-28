// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/AnimNotify_MontageStop.h"

UAnimNotify_MontageStop::UAnimNotify_MontageStop() :
BlendOutValue(0.5f),
bEnable(true)
{
	
}

void UAnimNotify_MontageStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if(AnimInstance && bEnable)
	{
		UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
		if(CurrentMontage)
		{
			AnimInstance->Montage_Stop(BlendOutValue, CurrentMontage);	
		}
	}
}

FString UAnimNotify_MontageStop::GetNotifyName_Implementation() const
{
	return FString("Montage Stop");
}
