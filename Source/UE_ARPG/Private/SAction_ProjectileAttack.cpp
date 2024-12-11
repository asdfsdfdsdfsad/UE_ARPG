// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include <Camera/CameraComponent.h>

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass)) {
	
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParames;
		SpawnParames.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParames.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Parames;
		Parames.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParames;
		ObjParames.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParames.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParames.AddObjectTypesToQuery(ECC_Pawn);

		//FVector TraceStart = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);;

		FVector V;
		FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();
		// Add sweep radius onto start to avoid the sphere clipping into floor/walls the camera is directly against.
		const FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * 20);
		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
		const FVector TraceEnd = TraceStart + (TraceDirection * 5000);
		V = TraceEnd;

		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParames, Shape, Parames))
		{
			V = Hit.ImpactPoint;
		}
		//FString FailedMsg = FString::Printf(TEXT("Hello"));
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
		//DrawDebugLine(GetWorld(), TraceStart, V, FColor::Red, false, 2.0f, 0, 2.0f);
		FRotator ProjRotation = FRotationMatrix::MakeFromX(V-HandLocation).Rotator();
		FTransform SpawnTM = FTransform(ProjRotation,HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass,SpawnTM,SpawnParames);
	}
	StopAction(InstigatorCharacter);
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{

	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character) {
		Character->PlayAnimMontage(AttackAnim);
		//UGameplayStatics::SpawnEmitterAttached(CastingEffect,Character->GetMesh(),HandSocketName,FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::SnapToTarget);
	    
		//FTimerHandle TimerHandle_AttackDelay;
		//FTimerDelegate Delegate;
		//Delegate.BindUFunction(this,"AttackDelay_Elapsed",Character);
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay,Delegate,AttackAnimDelay,false);
		AttackDelay_Elapsed(Character);
	}
	
}

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Cast_L";
	AttackAnimDelay = 1.0f;
}
