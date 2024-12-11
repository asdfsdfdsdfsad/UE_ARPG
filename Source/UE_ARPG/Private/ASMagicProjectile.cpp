// Fill out your copyright notice in the Description page of Project Settings.


#include "ASMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "SActionComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AASMagicProjectile::AASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AASMagicProjectile::OnActorOverlap);
	SphereComp->OnComponentHit.AddDynamic(this,&AASMagicProjectile::OnActorHit);
	RootComponent=SphereComp;
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	/*MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(SphereComp);*/
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");

	//ParticleSystem=ConstructorHelpers::FObjectFinder<UParticleSystem> (TEXT("'/Game/ParagonMinions/FX/Particles/Minions/Prime_Helix/Abilities/PrimaryAttack/FX/Helix_PrimaryImpact.Helix_PrimaryImpact'")).Object;
	
	MovementComp->InitialSpeed = 8000.0f;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	SetLifeSpan(10);
}

// Called when the game starts or when spawned
void AASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (AttackSound) 
	{
		UGameplayStatics::PlaySound2D(GetWorld(),AttackSound,1,1,12);
	}
	
}

void AASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor!=GetInstigator()){
		if (isOverride)
			return;
		isOverride = true;
		/*USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;
			return;
		}*/

		/*ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character) {
			int32 BoneIndex = Character->GetMesh()->GetBoneIndex(SweepResult.BoneName);
			if (BoneIndex != INDEX_NONE) {
				FString FailedMsg = FString::Printf(TEXT("2"));
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				Character->GetMesh()->SetSimulatePhysics(true);
				Character->GetMesh()->AddForce(GetActorForwardVector()*10000,SweepResult.BoneName,true);
			}
		}*/

		FVector V = SweepResult.Location-GetActorForwardVector()*200;
		FRotator R = GetActorRotation();
		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, -20.0f, SweepResult,GetActorLocation())) {
			if (ParticleSystem) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ParticleSystem,SweepResult.Location, R);
			}
		}

		//FString FailedMsg = FString::Printf(TEXT("2"));
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, V.ToString());
		isOverride = false;
		Destroy();
	}
	SetLifeSpan(1);
}

void AASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

// Called every frame
void AASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

