// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/SAI_LaneCharacter.h"
#include "AI/AIAttackComponent.h"
#include "Components/BoxComponent.h"
#include "SGameplayFunctionLibrary.h"

ASAI_LaneCharacter::ASAI_LaneCharacter()
{
	AttackComponent = CreateDefaultSubobject<UAIAttackComponent>("AttackComponent");
	CollisionBoxL = CreateDefaultSubobject<UBoxComponent>("CollisionBoxL");
	CollisionBoxR = CreateDefaultSubobject<UBoxComponent>("CollisionBoxR");
	CollisionBoxL->SetupAttachment(GetMesh(),FName("FX_Trail_03_L"));
	CollisionBoxR->SetupAttachment(GetMesh(),FName("FX_Trail_03_R"));
}

void ASAI_LaneCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ASAI_LaneCharacter::OnMontageEnd);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAI_LaneCharacter::OnMontageNotigy);

	CollisionBoxL->OnComponentBeginOverlap.AddDynamic(this, &ASAI_LaneCharacter::CollisionBoxBegainOverride);

	CollisionBoxR->OnComponentBeginOverlap.AddDynamic(this, &ASAI_LaneCharacter::CollisionBoxBegainOverride);

}

void ASAI_LaneCharacter::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	AttackComponent->EndAttack(this);
	GetMesh()->bUpdateOverlapsOnAnimationFinalize = false;
	CollisionBoxR->SetGenerateOverlapEvents(false);
	CollisionBoxL->SetGenerateOverlapEvents(false);
}

void ASAI_LaneCharacter::OnMontageNotigy(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName.IsEqual(FName("Attack1"))) {
		CollisionBoxR->SetGenerateOverlapEvents(true);
		GetMesh()->bUpdateOverlapsOnAnimationFinalize = true;
	}
	if (NotifyName.IsEqual(FName("StopAttack1"))) {
		CollisionBoxR->MoveIgnoreActors.Empty();
		CollisionBoxR->SetGenerateOverlapEvents(false);
		GetMesh()->bUpdateOverlapsOnAnimationFinalize = false;
	}
	if (NotifyName.IsEqual(FName("Attack2"))) {
		CollisionBoxL->SetGenerateOverlapEvents(true);
		GetMesh()->bUpdateOverlapsOnAnimationFinalize = true;
		
		//CollisionBoxL->UpdateOverlaps();
	}
	if (NotifyName.IsEqual(FName("StopAttack2"))) {
		CollisionBoxL->MoveIgnoreActors.Empty();
		CollisionBoxL->SetGenerateOverlapEvents(false);
		GetMesh()->bUpdateOverlapsOnAnimationFinalize = false;
	} 
	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("L=%d"),CollisionBoxL->ShouldSkipUpdateOverlaps()));
	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("R=%d"),CollisionBoxR->ShouldSkipUpdateOverlaps()));
	
}

void ASAI_LaneCharacter::CollisionBoxBegainOverride(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, SweepResult.GetActor()->GetName());
	if (SweepResult.GetActor() != this)
	{
		OverlappedComponent->IgnoreActorWhenMoving(SweepResult.GetActor(), true);
		FVector Hit = (OverlappedComponent->K2_GetComponentLocation() + SweepResult.GetActor()->GetActorLocation()) / 2;
		USGameplayFunctionLibrary::ApplyDirectionalDamage(this, SweepResult.GetActor(), -10, SweepResult,Hit);
	}
}
