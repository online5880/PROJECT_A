// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/Controller/MeleeEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AMeleeEnemyAIController::HomePosKey(TEXT("HomePos"));
const FName AMeleeEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));

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
	if(Blackboard)
	{
		Blackboard->InitializeBlackboard(*BlackboardData);
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
	}
}
