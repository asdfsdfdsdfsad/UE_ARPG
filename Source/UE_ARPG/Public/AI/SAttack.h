// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UE_ARPG_API USAttack : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,Category="Attack")
	FName AttackName;

	UFUNCTION(BlueprintNativeEvent,Category="Attack")
	void StartAttack(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void EndAttack(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void SkillAttack1();
};
