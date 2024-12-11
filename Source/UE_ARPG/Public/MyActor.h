// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInterface.h"
#include <SCharacter.h>
#include "MyActor.generated.h"
UCLASS()
class UE_ARPG_API AMyActor : public AActor,public ISInterface
{
	GENERATED_BODY()
	void Interact_Implementation(APawn* Instigatorpawn);
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
