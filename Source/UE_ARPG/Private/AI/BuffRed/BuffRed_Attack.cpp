// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BuffRed/BuffRed_Attack.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SGameplayFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"

void UBuffRed_Attack::StartAttack_Implementation(AActor* Instigator)
{
	Super::StartAttack_Implementation(Instigator);
	if (!Character) {
		Character = Cast<ACharacter>(Instigator);
		ActorsToIgnoreL.Add(Character);
	}

	switch (AttackIndex) 
	{
		case 0:
		{
			NormalAttack();
			AttackIndex = 1;
			USAttributeComponent* AttributeComp = Character->GetComponentByClass<USAttributeComponent>();
			AttributeComp->isSkillAttack = true;
		
		}
		break;
		case 1:
		{
			SkillAttack();
			AttackIndex = 0;
			USAttributeComponent* AttributeComp = Character->GetComponentByClass<USAttributeComponent>();
			AttributeComp->isSkillAttack = false;
		}
		break;
	}

}

void UBuffRed_Attack::EndAttack_Implementation(AActor* Instigator)
{
	Super::EndAttack_Implementation(Instigator);
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	ActorsToIgnoreL.Empty();
	ActorsToIgnoreR.Empty();
	//GEngine->AddOnScreenDebugMessage(-1,1.0f, FColor::Red, TEXT("D"));
}

void UBuffRed_Attack::SkillAttack1_Implementation()
{
	FHitResult HitResult;
	FVector AttackLocation = Character->GetMesh()->GetSocketLocation(TEXT("FX_Smash"));
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),AttackLocation,AttackLocation,50,TraceTypeQuery3, false, ActorsToIgnoreL, EDrawDebugTrace::None, HitResult, true);
	if (HitResult.bBlockingHit)
	{
		//GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,TEXT("321"));
		USGameplayFunctionLibrary::ApplyDirectionalDamage(Character, HitResult.GetActor(), -5, HitResult,AttackLocation);
	}

} 

void UBuffRed_Attack::NormalAttack()
{
	Character->PlayAnimMontage(NA_Montage);
}

void UBuffRed_Attack::SkillAttack()
{
	Character->PlayAnimMontage(SK_Montage);
}

void UBuffRed_Attack::AttackDecide()
{
	FHitResult HitResultL;
	FHitResult HitResultR;
	EndLocationL = Character->GetMesh()->GetSocketLocation(TEXT("FX_hand_l"));
	EndLocationR = Character->GetMesh()->GetSocketLocation(TEXT("FX_hand_r"));
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),StartLocationL,EndLocationL, 5, ETraceTypeQuery::TraceTypeQuery3, false, ActorsToIgnoreL, EDrawDebugTrace::Type::None, HitResultL, true, FLinearColor::Red);
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),StartLocationR,EndLocationR, 5, ETraceTypeQuery::TraceTypeQuery3, false, ActorsToIgnoreR, EDrawDebugTrace::Type::None, HitResultR, true, FLinearColor::Red);
	if (HitResultL.bBlockingHit){
		if (!ActorsToIgnoreL.Contains(HitResultL.GetActor()))
			ActorsToIgnoreL.Add(HitResultL.GetActor());
		USGameplayFunctionLibrary::ApplyDirectionalDamage(Character,HitResultL.GetActor(),-10,HitResultL, StartLocationL);
		//GEngine->AddOnScreenDebugMessage(-1,1.0f, FColor::Red,TEXT("L"));
	}
	if (HitResultR.bBlockingHit){
		if (!ActorsToIgnoreR.Contains(HitResultR.GetActor()))
			ActorsToIgnoreR.Add(HitResultR.GetActor());
		USGameplayFunctionLibrary::ApplyDirectionalDamage(Character, HitResultR.GetActor(), -10, HitResultR, StartLocationR);
		//GEngine->AddOnScreenDebugMessage(-1,1.0f, FColor::Red, TEXT("R"));
	}
	StartLocationL = EndLocationL;
	StartLocationR = EndLocationR;
}

void UBuffRed_Attack::SkillAttackDecide()
{
	FHitResult HitResultL;
	EndLocationL = Character->GetMesh()->GetSocketLocation(TEXT("FX_hand_l"));
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLocationL, EndLocationL, 5, ETraceTypeQuery::TraceTypeQuery3, false, ActorsToIgnoreL, EDrawDebugTrace::Type::None, HitResultL, true, FLinearColor::Red);
	if (HitResultL.bBlockingHit) {
		//if (!ActorsToIgnoreL.Contains(HitResultL.GetActor()))
			//ActorsToIgnoreL.Add(HitResultL.GetActor()); 
		UCharacterMovementComponent* MoveComp = HitResultL.GetActor()->GetComponentByClass<UCharacterMovementComponent>();
		MoveComp->Launch(FVector(0,0,1)*800);
		USGameplayFunctionLibrary::ApplyDirectionalDamage(Character, HitResultL.GetActor(),-10, HitResultL, StartLocationL);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("L"));
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void UBuffRed_Attack::OnAttackOverride(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,TEXT("122"));
}
