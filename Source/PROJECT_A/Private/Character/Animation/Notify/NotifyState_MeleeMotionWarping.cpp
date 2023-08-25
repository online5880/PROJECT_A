// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/NotifyState_MeleeMotionWarping.h"

#include "GlobalUtilty.h"
#include "MotionWarpingComponent.h"
#include "Character/Interface/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UNotifyState_MeleeMotionWarping::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	TArray<AActor*> Targets = FindNearTarget(MeshComp);

	// 공격을 시작할 때 주변에 있는 액터를 배열에 저장
	if(MeshComp->GetOwner())
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(MeshComp->GetOwner());
		if(CombatInterface)
		{
			//CombatInterface->MoveToTargets(Targets);
		}
	}
}

void UNotifyState_MeleeMotionWarping::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

FString UNotifyState_MeleeMotionWarping::GetNotifyName_Implementation() const
{
	return FString("Melee MotionWarping");
}

TArray<AActor*> UNotifyState_MeleeMotionWarping::FindNearTarget(const USkeletalMeshComponent* MeshComp) const
{
	const AActor* OwnerActor  = MeshComp->GetOwner();

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Emplace(MeshComp->GetOwner());

	TArray<FHitResult> OverlapHitResults;
	TArray<AActor*> OverlapActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(EnemyObjectType);
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		MeshComp->GetWorld(),
		MeshComp->GetComponentLocation(),
		MeshComp->GetComponentLocation()+FVector(0.1f),
		500.f,
		ObjectType,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		OverlapHitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f);
	
	for (FHitResult& OverlapHitResult : OverlapHitResults)
	{
		const AActor* Actor = OverlapHitResult.GetActor();
		if(Actor && !OverlapActors.Contains(Actor))
		{
			OverlapActors.Add(OverlapHitResult.GetActor());
		}
	}

	// Sort(Distance)
	OverlapActors.Sort([&](const AActor& A, const AActor& B)
	{
		return (A.GetActorLocation()- OwnerActor->GetActorLocation()).SizeSquared() <
				(B.GetActorLocation()-OwnerActor->GetActorLocation()).SizeSquared();
	});
	
	
	// Debug
	for (int i = 0; i < OverlapActors.Num(); ++i)
	{
		const AActor* Actor = OverlapActors[i];
		FVector ActorLocation = Actor->GetActorLocation();
		const float DistanceToPlayer = (ActorLocation - OwnerActor->GetActorLocation()).Size();
		const float ColorIntensity = 1.0f - FMath::Clamp(DistanceToPlayer / 500.f, 0.0f, 1.0f);
		
		FColor DebugColor = FColor(255 * ColorIntensity, 0, 0);
		DrawDebugSphere(
			OverlapActors[i]->GetWorld(),
			OverlapActors[i]->GetActorLocation(),
			30.f,
			8,
			DebugColor,
			false,
			0.5f,0,1.f);
	}

	if(OverlapActors.Num() > 0) {return OverlapActors;}

	return TArray<AActor*>();
}