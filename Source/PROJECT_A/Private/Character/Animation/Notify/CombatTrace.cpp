// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/CombatTrace.h"

#include "UtilityFunction.h"
#include "Character/Component/CombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UCombatTrace::UCombatTrace()
{
	AttackRadius = 30;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
}

void UCombatTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// World nullptr 이면 return
	const UWorld* World = MeshComp->GetWorld();
	if(World == nullptr) {return;}

	// 트레이스 시작 위치
	const FVector Start = MeshComp->GetSocketLocation(StartSocketName);
	// 트레이스 끝 위치
	const FVector End = MeshComp->GetSocketLocation(EndSocketName);

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		World,
		Start,
		End,
		AttackRadius,
		ObjectTypes,
		false,
		IgnoreActors,
		DrawDebugType(MeshComp),
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		DebugTime(MeshComp));
	
	for (const FHitResult& HitResult : HitResults)
	{
		PrintEditorMessage(3.f,HitResult.GetActor()->GetName());
	}
}

FString UCombatTrace::GetNotifyName_Implementation() const
{
	return FString("Combat Trace");
}

TEnumAsByte<EDrawDebugTrace::Type> UCombatTrace::DrawDebugType(const USkeletalMeshComponent* MeshComp) const
{
	const UCombatComponent* Component = GetCombatComponentFromMesh(MeshComp);
	return (Component && Component->GetDebug()) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
}

float UCombatTrace::DebugTime(const USkeletalMeshComponent* MeshComp) const
{
	const UCombatComponent* Component = GetCombatComponentFromMesh(MeshComp);
	return (Component) ? Component->GetDebugTime() : 5.f;
}

TObjectPtr<UCombatComponent> UCombatTrace::GetCombatComponentFromMesh(const USkeletalMeshComponent* MeshComp) const
{
	if (!MeshComp)
	{
		return nullptr;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	return Cast<UCombatComponent>(Owner->GetComponentByClass(UCombatComponent::StaticClass()));
}
