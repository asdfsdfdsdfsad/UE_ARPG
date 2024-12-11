// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIAttackComponent.generated.h"

class USAttack;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ARPG_API UAIAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	USAttack* Attacks;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	bool StartAttack(AActor* Instigator);
	UFUNCTION()
	bool EndAttack(AActor* Instigator);
	UFUNCTION()
	bool SkillAttack1();

	UFUNCTION(BlueprintCallable,Category="Attack")
	void SetAttackObject(TSubclassOf<USAttack> Attack);
};
