// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/SAI_SiegeCharacter.h"
#include "AI/AIAttackComponent.h"

ASAI_SiegeCharacter::ASAI_SiegeCharacter()
{
	AttackComponent = CreateDefaultSubobject<UAIAttackComponent>("AttackComponent");
}

void ASAI_SiegeCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ASAI_SiegeCharacter::OnMontageEnd);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ASAI_SiegeCharacter::OnMontageNotigy);
}

void ASAI_SiegeCharacter::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	AttackComponent->EndAttack(this);
}

void ASAI_SiegeCharacter::OnMontageNotigy(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	
}
