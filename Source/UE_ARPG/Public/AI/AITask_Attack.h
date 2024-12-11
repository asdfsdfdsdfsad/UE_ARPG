// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AITask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UAITask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);


};
