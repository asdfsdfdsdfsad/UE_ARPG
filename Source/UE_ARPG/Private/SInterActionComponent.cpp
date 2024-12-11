// Fill out your copyright notice in the Description page of Project Settings.


#include "SInterActionComponent.h"
#include "SInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"),false, TEXT("Global Damage Modifier for Attribute Componet."), ECVF_Cheat);


// Sets default values for this component's properties
USInterActionComponent::USInterActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInterActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInterActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USInterActionComponent::PrimaryInteract() {

	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;

	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

	FVector End = EyeLocation +(EyeRotation.Vector() * 500);

	//FHitResult Hit;
	//bool bBlockkingHit=GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation,End,ObjectQueryParams);

	float Radius = 30.0f;
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	bool bBlockkingHit=GetWorld()->SweepMultiByObjectType(Hits,EyeLocation,End,FQuat::Identity,ObjectQueryParams,Shape);
	
	FColor LineColor = bBlockkingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits) {

		if (bDebugDraw) {
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor) {

			if (HitActor->Implements<USInterface>()) {
					APawn* MyPawn = Cast<APawn>(MyOwner);
					ISInterface::Execute_Interact(HitActor, MyPawn);
			}

		}
		
	}
	if (bDebugDraw) {
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}
	
}
