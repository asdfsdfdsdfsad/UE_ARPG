// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree,TEXT("BehaviorTree is null"))) {
		RunBehaviorTree(BehaviorTree);
	}
	
	/*APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this,0);
	if (MyPawn) {
		GetBlackboardComponent()->SetValueAsVector("MovePoint", MyPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor",MyPawn);
		UE_LOG(LogTemp, Display, TEXT("SetBlackboard"));
	}*/
}
