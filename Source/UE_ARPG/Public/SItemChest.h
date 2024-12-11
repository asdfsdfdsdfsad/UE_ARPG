// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class UE_ARPG_API ASItemChest : public AActor,public ISInterface
{
	GENERATED_BODY()
	
	void Interact_Implementation(APawn* Instigatorpawn);

public:	

	UPROPERTY(VisibleAnywhere)
	float TargetPitch;

	// Sets default values for this actor's properties
	ASItemChest();

	int tag = 0;

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
