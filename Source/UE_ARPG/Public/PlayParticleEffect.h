// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlayParticleEffect.h"
#include "PlayParticleEffect.generated.h"

/**
 * 
 */
UCLASS()
class UE_ARPG_API UPlayParticleEffect : public UAnimNotify_PlayParticleEffect
{
	GENERATED_BODY()
public:
	UPlayParticleEffect();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
