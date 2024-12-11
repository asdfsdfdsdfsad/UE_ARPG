// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include <Kismet/KismetMathLibrary.h>

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,FVector HitLocation)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp) {
		return AttributeComp->ApplyHealthChange(DamageCauser,DamageAmount,HitLocation);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,const FHitResult& HitResult,FVector HitLocation)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount,HitLocation))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();

		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal*300000.0f,HitResult.ImpactPoint,HitResult.BoneName);
		}
		//FVector v=TargetActor->GetActorLocation() - HitResult.ImpactPoint;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,v.ToString());
		return true;
	}
	return false;
}

int USGameplayFunctionLibrary::AttackLocation(FVector Target, FRotator TargetR, FVector HitTarget)
{
	FVector Hit = (HitTarget + Target) / 2;
	FRotator R = UKismetMathLibrary::FindLookAtRotation(Target,Hit);
	float f[6];
	float a = TargetR.Yaw;
	float b = TargetR.Yaw;
	for (int i = 0; i < 6; i += 2) {
		a = a - 45;
		b = b + 45;
		a = a < -180 ? 360 + a : a;
		b = b > 180 ? b - 360 : b;
		f[i] = a;
		f[i + 1] = b;
	}
	if (R.Yaw > f[0] && R.Yaw < f[1])
	{
		return 1;//Forward
	}
	else if (R.Yaw >= f[4] && R.Yaw <= f[0])
	{
		return 2;//Left
	}
	else if (R.Yaw <= f[5] && R.Yaw >= f[1])
	{
		return 3;//Right
	}
	else
	{
		return 4;//back
	}
	return 0;
}
