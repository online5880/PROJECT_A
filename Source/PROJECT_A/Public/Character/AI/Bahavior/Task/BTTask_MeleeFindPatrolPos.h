// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MeleeFindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_A_API UBTTask_MeleeFindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MeleeFindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
