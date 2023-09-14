// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeEnemyAIController.generated.h"

class AEnemyBase;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class PROJECT_A_API AMeleeEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMeleeEnemyAIController();

	// key
	/**
	 * @brief 처음 시작 위치
	 */
	static const FName HomePosKey;
	/**
	 * @brief 정찰 위치
	 */
	static const FName PatrolPosKey;

	/**
	 * @brief 타겟 키
	 */
	static const FName TargetKey;
	
	void SetTarget(AActor* Actor);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * @brief 블랙보드 초기화
	 * @param InPawn Owner Pawn
	 * @param BlackboardComp BlackBoard Component
	 */
	void InitBlackBoard(APawn* InPawn, UBlackboardComponent* BlackboardComp);

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardData> BlackboardData;

	UPROPERTY()
	TObjectPtr<AEnemyBase> EnemyBase;
};
