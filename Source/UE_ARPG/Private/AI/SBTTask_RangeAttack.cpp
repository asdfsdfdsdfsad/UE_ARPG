// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController)) {
	
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr) {
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_Front");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	    
		if (TargetActor == nullptr) {
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation();
		FRotator MuzzleRotation = (Direction-MuzzleLocation).Rotation();
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass,MuzzleLocation,MuzzleRotation,Params);
		FVector RVector = Direction - MyPawn->GetActorLocation();
		double x1 = RVector.X;
		double y1 = RVector.Y;
		RVector *= RVector;
		double s=FMath::Sqrt(RVector.X+RVector.Y);
		s = (FMath::Acos(x1 / s) * 180 / 3.1415926);
		s = y1 > 0 ? s : -s;
		FRotator v = MyPawn->GetActorRotation();
		v.Yaw = s;
		//MyPawn->SetActorRotation(v);
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Type();
}
