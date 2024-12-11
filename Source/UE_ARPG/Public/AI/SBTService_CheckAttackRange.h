// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API USBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere,Category="AI")
	FBlackboardKeySelector AttackRangeKey;
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRange;
};
