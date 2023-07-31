// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/CombatTrace.h"

#include "UtilityFunction.h"
#include "Character/Component/CombatComponent.h"
#include "Character/Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UCombatTrace::UCombatTrace()
{
	AttackRadius = 30;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}



void UCombatTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	// World nullptr 이면 return
	const UWorld* World = MeshComp->GetWorld();
	if(World == nullptr) {return;}
	
	PlayAttackSound(World);
	IgnoreActors.Emplace(MeshComp->GetOwner());
}

void UCombatTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	// World nullptr 이면 return
	const UWorld* World = MeshComp->GetWorld();
	if(World == nullptr) {return;}

	// 트레이스 시작 위치
	const FVector Start = MeshComp->GetSocketLocation(StartSocketName);
	// 트레이스 끝 위치
	const FVector End = MeshComp->GetSocketLocation(EndSocketName)+FVector(0.00001);
	
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
		AActor* HitActor = HitResult.GetActor();
		if(HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
			IgnoreActors.AddUnique(HitActor);
			
			DrawDebugDirectionalArrow(
				World,
				HitResult.ImpactPoint,
				HitResult.ImpactPoint+HitResult.ImpactNormal*-100.f,
				10.f,
				FColor::Orange,
				false,
				5.f,
				0,
				1.f);
		}
	}
	
	ExecuteDamagedOnHitActors(HitActors);
}

void UCombatTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ExecuteEndDamagedOnHitActors(HitActors);

	// 배열들 초기화
	HitActors.Empty();
	IgnoreActors.Empty();
	HitResults.Empty();
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

void UCombatTrace::PlayAttackSound(const UWorld* World) const
{
	if(World && AttackSound)
	{
		UGameplayStatics::PlaySound2D(World,AttackSound,AttackSoundVolume);	
	}
}

void UCombatTrace::ExecuteDamagedOnHitActors(const TSet<TObjectPtr<AActor>>& HitActorArr)
{
	for (const TObjectPtr<AActor>& HitActor : HitActorArr)
	{
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
		{
			ICombatInterface::Execute_Damaged(HitActor);
		}
	}
}

void UCombatTrace::ExecuteEndDamagedOnHitActors(const TSet<TObjectPtr<AActor>>& HitActorArr)
{
	for (TObjectPtr<AActor> HitActor : HitActorArr)
	{
		if(HitActor->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
		{
			ICombatInterface::Execute_EndDamaged(HitActor);
		}
	}
}