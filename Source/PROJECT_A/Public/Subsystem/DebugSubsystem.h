// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_A_API UDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDebugSubsystem();

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "DebugSubsystem")
	bool bDebug;
};
