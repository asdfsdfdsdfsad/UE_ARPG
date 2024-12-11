// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SAICharacter.h"
#include "SAI_LaneCharacter.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class UE_ARPG_API ASAI_LaneCharacter : public ASAICharacter
{
	GENERATED_BODY()
protected:
	ASAI_LaneCharacter();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void OnMontageNotigy(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIAttackComponent* AttackComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UBoxComponent* CollisionBoxL;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UBoxComponent* CollisionBoxR;

	UFUNCTION()
	void CollisionBoxBegainOverride(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
