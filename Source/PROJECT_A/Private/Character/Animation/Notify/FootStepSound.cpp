// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/FootStepSound.h"

#include "UtilityFunction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UFootStepSound::UFootStepSound()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>
	DT_FootStep(TEXT("/Script/Engine.DataTable'/Game/_Game/Character/ROGAN/Data/DT_FootStep.DT_FootStep'"));
	
	if(DT_FootStep.Succeeded()) {DT_FootStepSound = DT_FootStep.Object;}
}

void UFootStepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor)
	{
		UWorld* World = OwnerActor->GetWorld();
		const FVector Start = OwnerActor->GetActorLocation();
		const FVector End = Start - FVector(0.f, 0.f, 100.f);

		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(OwnerActor);

		bool bHit = UKismetSystemLibrary::LineTraceSingle(
			World,
			Start,
			End,
			TraceTypeQuery1,
			false,
			IgnoreActors,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true);

		if (bHit && DT_FootStepSound)
		{
			UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get();
			if (PhysMaterial)
			{
				EPhysicalSurface SurfaceType = PhysMaterial->SurfaceType;
				FString SurfaceTypeString = UEnum::GetValueAsString<EPhysicalSurface>(SurfaceType);
				FFootStepProperty* FootStepProperty = DT_FootStepSound->FindRow<FFootStepProperty>(*SurfaceTypeString, FString(""));
				
				if (FootStepProperty)
				{
					PlaySound2D(World, FootStepProperty->Sound);
				}
				if(bDebugSurfaceType)
				{
					PrintEditorMessage(1.f,FString("SurfaceType : ")+SurfaceTypeString);
				}
			}
		}
	}
}

FString UFootStepSound::GetNotifyName_Implementation() const
{
	return FString("Foot Step");
}
