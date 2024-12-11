// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "Components/TimelineComponent.h"
#include "SAction_Capture.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API USAction_Capture : public USAction
{
	GENERATED_BODY()
private:
	UPROPERTY()
	FTimerHandle TimeHandle;
	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector EndLocation;
	UPROPERTY(EditAnywhere,Category = "SweepTarget")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	UPROPERTY(EditAnywhere,Category = "SweepTarget")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY()
	ACharacter* Character;

	UPROPERTY()
	AActor* Actor;

	UPROPERTY(EditAnywhere,Category="String")
	TSubclassOf<AActor> StringActor;

	UPROPERTY()
	AActor* StringA;

	UPROPERTY()
	float Yaw;
	UPROPERTY()
	float Pitch;

	//时间轴
	UPROPERTY()
	UTimelineComponent* Timeline;

	//曲线向量
	UPROPERTY()
	UCurveVector* CurveVector;

	UPROPERTY()
	bool b = true;


	UFUNCTION()
	void FindTargetLocation();
	USAction_Capture();

	UFUNCTION()
	void OnTimelineUpdate(FVector vector);
	UFUNCTION()
	void OnTimelineFinished();
public:

	//时间轴委托
	//UPROPERTY(BlueprintAssignable,Category="Timeline")
	FOnTimelineVectorStatic OnTimelineVector;

	UPROPERTY()
	FHitResult HitResult;

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
};
