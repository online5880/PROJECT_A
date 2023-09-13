// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Controller/MeleeEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/EnemyBase.h"

const FName AMeleeEnemyAIController::HomePosKey(TEXT("HomePos"));
const FName AMeleeEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMeleeEnemyAIController::TargetKey(TEXT("TargetKey"));

AMeleeEnemyAIController::AMeleeEnemyAIController()
{
}

void AMeleeEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
}

void AMeleeEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	InitBlackBoard(InPawn, BlackboardComp);
}

void AMeleeEnemyAIController::InitBlackBoard(APawn* InPawn, UBlackboardComponent* BlackboardComp)
{
	if(UseBlackboard(BlackboardData,BlackboardComp))
	{
		if(GetBlackboardComponent())
		{
			GetBlackboardComponent()->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
			GetBlackboardComponent()->InitializeBlackboard(*BlackboardData);
		}
	}
}

void AMeleeEnemyAIController::SetTarget()
{
	if(EnemyBase && EnemyBase->GetTarget())
	{
		GetBlackboardComponent()->SetValueAsObject(TargetKey, EnemyBase.Get()->GetTarget());	
	}
}
