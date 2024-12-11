// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SAttack.h"
#include "BuffRed_Attack.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UBuffRed_Attack : public USAttack
{
	GENERATED_BODY()
public:
	virtual void StartAttack_Implementation(AActor* Instigator) override;
	virtual void EndAttack_Implementation(AActor* Instigator) override;
	virtual void SkillAttack1_Implementation() override;
	UFUNCTION()
	void NormalAttack();

	void SkillAttack();

	//ÆÕÍ¨¹¥»÷
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Montage")
	UAnimMontage* NA_Montage;
	//¼¼ÄÜ¹¥»÷
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* SK_Montage;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UFUNCTION()
	void AttackDecide();
	UFUNCTION()
	void SkillAttackDecide();

	UPROPERTY()
	ACharacter* Character;

	UPROPERTY()
	FVector StartLocationL;
	UPROPERTY()
	FVector EndLocationL;

	UPROPERTY()
	FVector StartLocationR;
	UPROPERTY()
	FVector EndLocationR;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnoreL;
	UPROPERTY()
	TArray<AActor*> ActorsToIgnoreR;

	int AttackIndex = 0;

	UFUNCTION()
	void OnAttackOverride(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
