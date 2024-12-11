// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AITask_Attack.h"
#include "AIController.h"
#include "AI/AIAttackComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UAITask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (ensure(Controller)) {
		UAIAttackComponent* AttackComp = Controller->GetPawn()->GetComponentByClass<UAIAttackComponent>();
		UBlackboardComponent* TreeComp = OwnerComp.GetBlackboardComponent();
		if (AttackComp == nullptr) {
			return EBTNodeResult::Failed;
		}
        
		AttackComp->StartAttack(Controller->GetPawn());
		
	}
	return EBTNodeResult::Type();
}
