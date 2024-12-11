// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIAttackComponent.h"
#include "AI/SAttack.h"

// Sets default values for this component's properties
UAIAttackComponent::UAIAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAIAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAIAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAIAttackComponent::StartAttack(AActor* Instigator)
{
	if (Attacks) {
		Attacks->StartAttack(Instigator);
	}

	return false;
}

bool UAIAttackComponent::EndAttack(AActor* Instigator)
{
	if (Attacks) {
		Attacks->EndAttack(Instigator);
	}
	return false;
}

bool UAIAttackComponent::SkillAttack1()
{
	if (Attacks) {
		Attacks->SkillAttack1();
	}
	return false;
}

void UAIAttackComponent::SetAttackObject(TSubclassOf<USAttack> Attack)
{
	if (!ensure(Attack))
		return;
	Attacks = NewObject<USAttack>(this,Attack);
}

