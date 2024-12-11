// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayParticleEffect.h"

UPlayParticleEffect::UPlayParticleEffect()
{
	
}

void UPlayParticleEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//PSTemplate = nullptr;
	for (int i = 0; i < Animation->Notifies.Num(); i++) {
	
	}
	
}
