// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SAICharacter.h"
#include "SAI_BuffRedCharacter.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class UE_ARPG_API ASAI_BuffRedCharacter : public ASAICharacter
{
	GENERATED_BODY()
protected:

	ASAI_BuffRedCharacter();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void OnMontageNotigy(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	UAIAttackComponent* AttackComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	USphereComponent* SphereComponentL;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	USphereComponent* SphereComponentR;

	bool isSkill=false;


	UFUNCTION()
	void CollisionSphereBegainOverride(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
