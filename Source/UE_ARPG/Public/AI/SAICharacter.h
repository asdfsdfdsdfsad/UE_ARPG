// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class USWorldUserWidget;
class UUserWidget;
class UAIAttackComponent;
 
UCLASS()
class UE_ARPG_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	UPROPERTY()
	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly,Category="UI");
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere,Category="Particle")
	UParticleSystem* DealthParticle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor ,USAttributeComponent* OwningComp, float NewHealth,float Delta,FVector HitLocation);

	UPROPERTY(EditAnywhere, Category = "Hit")
	UAnimMontage* ForwardHitMontage;
	UPROPERTY(EditAnywhere, Category = "Hit")
	UAnimMontage* LeftHitMontage;
	UPROPERTY(EditAnywhere, Category = "Hit")
	UAnimMontage* RigthHitMontage;
	UPROPERTY(EditAnywhere, Category = "Hit")
	UAnimMontage* BackHitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* DealthSound;

	UFUNCTION()
	void DamagePlayMontage(int index);
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComponent;

	UFUNCTION()
	void SetAttackTarget(AActor* target);

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
