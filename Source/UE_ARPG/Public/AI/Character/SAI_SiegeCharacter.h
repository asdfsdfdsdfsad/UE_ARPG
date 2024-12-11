// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SAICharacter.h"
#include "SAI_SiegeCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API ASAI_SiegeCharacter : public ASAICharacter
{
	GENERATED_BODY()
protected:

	ASAI_SiegeCharacter();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void OnMontageNotigy(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIAttackComponent* AttackComponent;
};
