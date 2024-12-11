// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include <SGameModeBase.h>

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"),1.0f, TEXT("Global Damage Modifier for Attribute Componet."), ECVF_Cheat);


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100;
	// ...
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool USAttributeComponent::isAlive()
{
	if (Health <= 0)
	{
		if (OnDeath.IsBound()) {
			OnDeath.ExecuteIfBound(true);
		}
	}
	return Health>0.0f;
}

bool USAttributeComponent::ApplyHealthChange(AActor* AttackTarget,float Delta,FVector HitLocation)
{
	Health += Delta;
	OnHealthChanged.Broadcast(AttackTarget,this,Health,Delta,HitLocation);
	if (Delta<0.0f) {
	
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	if (Health == 0.0f) {
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM) {
			GM->OnActorKilled(GetOwner(),AttackTarget);
		}
	}

	return true;
}

bool USAttributeComponent::isAlives(AActor* Actor)
{
	USAttributeComponent* att = Cast<USAttributeComponent>(Actor->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (att)
	{
		return att->isAlive();
	}
	return false;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* Actor)
{
	USAttributeComponent* Attribute =Actor->GetComponentByClass<USAttributeComponent>();
	if (Attribute) {
		return Attribute;
	}
	return nullptr;
}

void USAttributeComponent::SetHealth(float Value)
{
	Health = Value;
}

// Called every frame
void USAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

