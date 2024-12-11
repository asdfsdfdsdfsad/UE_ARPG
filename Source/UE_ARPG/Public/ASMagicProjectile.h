// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "ASMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;


UCLASS()
class UE_ARPG_API AASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASMagicProjectile();

protected:

	bool isOverride=false;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag ParryTag;

	UPROPERTY(VisibleAnywhere)
	USphereComponent * SphereComp;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ParticleSystem;

	/*UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Mesh")
	UStaticMeshComponent* MeshComponent;*/

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* AttackSound;
public:	

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
