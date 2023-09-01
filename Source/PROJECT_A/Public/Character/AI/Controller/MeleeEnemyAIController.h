// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_A_API AMeleeEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMeleeEnemyAIController();

protected:
	virtual void BeginPlay() override;
	
};
