// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SAttack.h"
#include "MinionLane.generated.h"

class UCurveVector;
/**
 * 
 */
UCLASS()
class UE_ARPG_API UMinionLane : public USAttack
{
	GENERATED_BODY()

	virtual void StartAttack_Implementation(AActor* Instigator) override;
	virtual void EndAttack_Implementation(AActor* Instigator) override;
	virtual void SkillAttack1_Implementation() override;

protected:
	UFUNCTION()
	void NormalAttack();

	UFUNCTION()
	void Attack(float timer=0);

	UFUNCTION()
	void TimerEvent();
	UFUNCTION()
	void ClearTimerHandle();

	UPROPERTY()
	ACharacter* Character;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Montage")
	UAnimMontage* Montage;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY()
	TArray<FVector> VectorArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	UCurveVector* CurveVectors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	UCurveFloat* CurveFloat;

	UPROPERTY()
	FVector StartVector;
	UPROPERTY()
	FVector StartVector2;

	UPROPERTY()
	TArray<FQuat> QuatArray;

	UPROPERTY()
	FName SocketName1;

	UPROPERTY()
	FName SocketName2;

	int sum = 1;

	int AttackCount = 4;

	bool isAttack = true;
};
