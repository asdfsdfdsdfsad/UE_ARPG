// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SAttack.h"
#include "MinionSiege.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UMinionSiege : public USAttack
{
	GENERATED_BODY()
	
	virtual void StartAttack_Implementation(AActor* Instigator) override;
	virtual void EndAttack_Implementation(AActor* Instigator) override;
	virtual void SkillAttack1_Implementation() override;

	UFUNCTION()
	void NormalAttack();

	UPROPERTY()
	ACharacter* Character;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

};
