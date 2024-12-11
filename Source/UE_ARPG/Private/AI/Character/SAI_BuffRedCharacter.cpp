// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/SAI_BuffRedCharacter.h"
#include "AI/AIAttackComponent.h"
#include "Components/SphereComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ASAI_BuffRedCharacter::ASAI_BuffRedCharacter()
{
	AttackComponent = CreateDefaultSubobject<UAIAttackComponent>("AttackComponent");
	SphereComponentL = CreateDefaultSubobject<USphereComponent>("SphereCompL");
	SphereComponentR = CreateDefaultSubobject<USphereComponent>("SphereCompR");
	SphereComponentL->SetupAttachment(GetMesh(),FName("FX_hand_l"));
	SphereComponentR->SetupAttachment(GetMesh(),FName("FX_hand_r"));
}

void ASAI_BuffRedCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ASAI_BuffRedCharacter::OnMontageEnd);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAI_BuffRedCharacter::OnMontageNotigy);
	//SphereComponentL->IgnoreActorWhenMoving(this,true);
	//SphereComponentR->IgnoreActorWhenMoving(this,true);
	SphereComponentL->OnComponentBeginOverlap.AddDynamic(this, &ASAI_BuffRedCharacter::CollisionSphereBegainOverride);
	SphereComponentR->OnComponentBeginOverlap.AddDynamic(this, &ASAI_BuffRedCharacter::CollisionSphereBegainOverride);

}

void ASAI_BuffRedCharacter::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	AttackComponent->EndAttack(this);
}

void ASAI_BuffRedCharacter::OnMontageNotigy(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	int a = NotifyName.GetNumber();
	switch (a) {
	case 1:
	{
		SphereComponentL->SetGenerateOverlapEvents(true);
		GetMesh()->bUpdateOverlapsOnAnimationFinalize = true;
		FString str = NotifyName.GetPlainNameString();
		if (str.Equals("AttackL_Skill"))
		{
			isSkill = true;
		}
	}
		break;
	case 2:
		SphereComponentL->SetGenerateOverlapEvents(false);
		GetMesh()->bUpdateOverlapsOnAnimationFinalize = false;
		SphereComponentL->MoveIgnoreActors.Empty();
		break;
	case 3:
		{	
			SphereComponentR->SetGenerateOverlapEvents(true);
			GetMesh()->bUpdateOverlapsOnAnimationFinalize = true;
	    }
		break;
	case 4:
		SphereComponentR->SetGenerateOverlapEvents(false);
		GetMesh()->bUpdateOverlapsOnAnimationFinalize = false;
		SphereComponentR->MoveIgnoreActors.Empty();
		break;
	case 5:
		AttackComponent->SkillAttack1();
		break;
	}
}

void ASAI_BuffRedCharacter::CollisionSphereBegainOverride(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (SweepResult.GetActor() != this)
	{
		OverlappedComponent->IgnoreActorWhenMoving(SweepResult.GetActor(), true);
		if (isSkill) {
			isSkill = false;
			UCharacterMovementComponent* MoveComp = SweepResult.GetActor()->GetComponentByClass<UCharacterMovementComponent>();
			MoveComp->Launch(FVector(0, 0, 1) * 800);
		}
		FVector Hit = (OverlappedComponent->K2_GetComponentLocation() + SweepResult.GetActor()->GetActorLocation()) / 2;
		USGameplayFunctionLibrary::ApplyDirectionalDamage(this, SweepResult.GetActor(), -10, SweepResult, Hit);
	}
}
