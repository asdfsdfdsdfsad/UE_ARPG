// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="Gameplay")
	static bool ApplyDamage(AActor* DamageCauser,AActor* TargetActor,float DamageAmount,FVector HitLocation);

	UFUNCTION(BlueprintCallable,Category="Gameplay")
	static bool ApplyDirectionalDamage(AActor* DamageCauser,AActor* TargetActor,float DamageAmount,const FHitResult& HitResult,FVector HitLocation);

	UFUNCTION(BlueprintCallable,Category="Gameplay")
	static int AttackLocation(FVector Target, FRotator TargetR, FVector HitTarget);
};
