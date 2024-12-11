// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Minion/MinionLane.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Curves/CurveVector.h"

void UMinionLane::StartAttack_Implementation(AActor* Instigator)
{
	Super::StartAttack_Implementation(Instigator);
	if (!Character) {
		Character = Cast<ACharacter>(Instigator);
	}
	NormalAttack();
}

void UMinionLane::EndAttack_Implementation(AActor* Instigator)
{
	Super::EndAttack_Implementation(Instigator);
	ClearTimerHandle();
}

void UMinionLane::SkillAttack1_Implementation()
{
	Super::SkillAttack1_Implementation();
}

void UMinionLane::NormalAttack()
{
	Character->PlayAnimMontage(Montage);
	//SocketName1 = TEXT("FX_Trail_01_R");
	//SocketName2 = TEXT("FX_Trail_02_R");
	//Attack(0.4);
}

void UMinionLane::Attack(float timer)
{
	//StartVector = (Character->GetMesh()->GetSocketLocation(SocketName1) + Character->GetMesh()->GetSocketLocation(SocketName2)) / 2;
	StartVector = Character->GetMesh()->GetSocketLocation(SocketName1);
	Character->GetWorldTimerManager().SetTimer(TimerHandle, this, &UMinionLane::TimerEvent,0.05,true,timer);
}

void UMinionLane::TimerEvent()
{
	//FVector V = (Character->GetMesh()->GetSocketLocation(SocketName1) + Character->GetMesh()->GetSocketLocation(SocketName2)) / 2;
	FVector V = Character->GetMesh()->GetSocketLocation(SocketName1);
	FRotator R = Character->GetMesh()->GetSocketRotation(SocketName1);
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	//UKismetSystemLibrary::BoxTraceSingle(GetWorld(),StartVector,V,FVector(3,25,3),R, ETraceTypeQuery::TraceTypeQuery3,false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);
	DrawDebugLine(GetWorld(),StartVector,V,FColor::Red,false,2);
	if (HitResult.bBlockingHit) 
	{
		GEngine->AddOnScreenDebugMessage(-1,1,FColor::Red,TEXT("hello"));
	}
	StartVector = V;
	//AttackCount--;
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("attck"));
	//if (AttackCount <=0) {
	//	Character->GetWorldTimerManager().ClearTimer(TimerHandle);
	//	//AttackCount = 4;
	//	if (isAttack) {
	//		isAttack = false;
	//		SocketName1 = TEXT("FX_Trail_01_L");
	//		SocketName2 = TEXT("FX_Trail_02_L");
	//		Attack(0);
	//	}
	//}
}

void UMinionLane::ClearTimerHandle()
{
	if (Character) 
	{
		Character->GetWorldTimerManager().ClearTimer(TimerHandle);
	}
	AttackCount = 5;
	isAttack = true;
}
