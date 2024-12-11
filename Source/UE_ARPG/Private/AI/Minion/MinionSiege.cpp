// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Minion/MinionSiege.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

void UMinionSiege::StartAttack_Implementation(AActor* Instigator)
{
	Super::StartAttack_Implementation(Instigator);
	if (!Character) {
		Character = Cast<ACharacter>(Instigator);
	}
	NormalAttack();
}

void UMinionSiege::EndAttack_Implementation(AActor* Instigator)
{
	Super::EndAttack_Implementation(Instigator);

}

void UMinionSiege::SkillAttack1_Implementation()
{
	Super::SkillAttack1_Implementation();

}

void UMinionSiege::NormalAttack()
{
	if (ensure(Character)) {
		AAIController* MyController = Cast<AAIController>(Character->GetController());

		if (MyController) {
			FVector MuzzleLocation = Character->GetMesh()->GetSocketLocation("Muzzle_Front");
			AActor* TargetActor = Cast<AActor>(MyController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));

			FVector Direction = TargetActor->GetActorLocation();
			FRotator MuzzleRotation = (Direction - MuzzleLocation).Rotation();
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			Params.Instigator = Character;
			AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
			FVector RVector = Direction - Character->GetActorLocation();
			double x1 = RVector.X;
			double y1 = RVector.Y;
			RVector *= RVector;
			double s = FMath::Sqrt(RVector.X + RVector.Y);
			s = (FMath::Acos(x1 / s) * 180 / 3.1415926);
			s = y1 > 0 ? s : -s;
			FRotator v = Character->GetActorRotation();
			v.Yaw = s;
			Character->SetActorRotation(v);
		}
	}


}
