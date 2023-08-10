// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/MeleeTrace.h"

#include "UtilityFunction.h"
#include "Character/Component/CombatComponent.h"
#include "Character/Enemy/EnemyBase.h"
#include "Character/Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UMeleeTrace::UMeleeTrace()
{
	AttackRadius = 30;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}



void UMeleeTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	IgnoreActors.Emplace(MeshComp->GetOwner());

	TArray<FHitResult> OverlapHitResults;
	TSet<AActor*> OverlapActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> EnemyObjectType;
	EnemyObjectType.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		MeshComp->GetWorld(),
		MeshComp->GetComponentLocation(),
		MeshComp->GetComponentLocation()+FVector(0.1f),
		500.f,
		EnemyObjectType,
		false,
		IgnoreActors,
		DrawDebugType(MeshComp),
		OverlapHitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		DebugTime(MeshComp));
	
	for (FHitResult& OverlapHitResult : OverlapHitResults)
	{
		const AActor* Actor = OverlapHitResult.GetActor();
		if(Actor && !OverlapActors.Contains(Actor))
		{
			OverlapActors.Add(OverlapHitResult.GetActor());
		}
	}
	
	for (const AActor* OverlapActor : OverlapActors)
	{
		PrintEditorMessage(3.f,OverlapActor->GetName());
	}
	if(!OverlapHitResults.IsEmpty())
	{
		TArray<AActor*> Arr;
		Arr.Append(OverlapActors.Array());
		int32 RandomIndex = FMath::RandRange(0, Arr.Num() - 1);
		MeshComp->GetOwner()->SetActorLocation(Arr[RandomIndex]->GetActorLocation());
	}
}

void UMeleeTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
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
				HitResult.ImpactPoint+(HitResult.ImpactNormal*FVector(1.f,1.f,0.f))*-100.f,
				20.f,
				FColor::Orange,
				false,
				5.f,
				0,
				2.f);

			ExecuteDamagedOnHitActors(HitActors,HitResult.ImpactNormal);
		}
	}
}

void UMeleeTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ExecuteEndDamagedOnHitActors(HitActors);

	// 배열들 초기화
	HitActors.Empty();
	IgnoreActors.Empty();
	HitResults.Empty();
}

FString UMeleeTrace::GetNotifyName_Implementation() const
{
	return FString("Melee Trace");
}

TEnumAsByte<EDrawDebugTrace::Type> UMeleeTrace::DrawDebugType(const USkeletalMeshComponent* MeshComp) const
{
	const UCombatComponent* Component = GetCombatComponentFromMesh(MeshComp);
	return (Component && Component->GetDebug()) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
}

float UMeleeTrace::DebugTime(const USkeletalMeshComponent* MeshComp) const
{
	const UCombatComponent* Component = GetCombatComponentFromMesh(MeshComp);
	return (Component) ? Component->GetDebugTime() : 5.f;
}

TObjectPtr<UCombatComponent> UMeleeTrace::GetCombatComponentFromMesh(const USkeletalMeshComponent* MeshComp) const
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

void UMeleeTrace::PlayAttackSound(const UWorld* World) const
{
	if(World && AttackSound)
	{
		UGameplayStatics::PlaySound2D(World,AttackSound,AttackSoundVolume);	
	}
}

void UMeleeTrace::ExecuteDamagedOnHitActors(const TSet<TObjectPtr<AActor>>& HitActorArr,const FVector& Normal)
{
	for (const TObjectPtr<AActor>& HitActor : HitActorArr)
	{
		//if (HitActor && HitActor->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
		if (HitActor && HitActor->GetComponentByClass(UCombatComponent::StaticClass()))
		{
			ICombatInterface* Interface = Cast<ICombatInterface>(HitActor);
			if(Interface)
			{
				Interface->Damaged(AttackDamage,Normal,AttackPower);
			}
		}
	}
}
void UMeleeTrace::ExecuteEndDamagedOnHitActors(const TSet<TObjectPtr<AActor>>& HitActorArr)
{
	for (TObjectPtr<AActor> HitActor : HitActorArr)
	{
		if(HitActor->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
		{
			ICombatInterface* Interface = Cast<ICombatInterface>(HitActor);
			if(Interface)
			{
				Interface->EndDamaged();
			}
		}
	}
}