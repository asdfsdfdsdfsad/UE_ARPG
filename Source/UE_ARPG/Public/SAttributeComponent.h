// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHealthChanged,AActor*,InstigatorActor,USAttributeComponent*,OwningComp,float,NewHealth,float,Delta,FVector,HitLocation);
DECLARE_DELEGATE_OneParam(FOnDeath,bool);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ARPG_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes");
	float Health;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool isSkillAttack = false;

	UFUNCTION(BlueprintCallable)
	bool isAlive();

	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool ApplyHealthChange(AActor* AttackTarget,float Delta,FVector HitLocation);

	UFUNCTION()
	static bool isAlives(AActor* Actor);

	UFUNCTION()
	static USAttributeComponent* GetAttributes(AActor* Actor);

	FOnDeath OnDeath;

	UFUNCTION()
	void SetHealth(float Value);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
