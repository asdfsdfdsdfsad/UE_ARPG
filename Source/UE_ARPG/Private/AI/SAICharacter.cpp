// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "AI/AIAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");
	AttributeComponent->OnHealthChanged.AddDynamic(this,&ASAICharacter::OnHealthChanged);
	// Enabled on mesh to react to incoming projectiles
	USkeletalMeshComponent* SkelMesh = GetMesh();
	SkelMesh->SetGenerateOverlapEvents(true);
	// Skip performing overlap queries on the Physics Asset after animation (17 queries in case of our MinionRangedBP)
	SkelMesh->bUpdateOverlapsOnAnimationFinalize = false;

	// Skip bones when not visible, may miss anim notifies etc. if animation is skipped so these options must be tested per use case
	SkelMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	TimeToHitParamName = "TimeOfHit";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ASAICharacter::OnPawnSeen);
}

// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp) {
		MoveComp->MaxWalkSpeed = 300;
		MoveComp->MaxWalkSpeedCrouched = 150;
	}
	//GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
}

/// <summary>
/// 委托，看见目标触发
/// </summary>
/// <param name="Pawn">目标</param>
void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		BBComp->SetValueAsObject("TargetActor",Pawn);
		//GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,Pawn->GetName());
	}
}

//伤害应用委托
void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta,FVector HitLocation)
{

	SetAttackTarget(InstigatorActor);
	if (ActiveHealthBar == NULL) {
		ActiveHealthBar=CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar) {
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
			AttributeComponent->OnHealthChanged.Broadcast(InstigatorActor,OwningComp,NewHealth,Delta,HitLocation);
			UE_LOG(LogTemp,Log,TEXT("BindHealthUI"));
		}
	}

	int index = USGameplayFunctionLibrary::AttackLocation(GetActorLocation(),GetActorRotation(),HitLocation);

	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DealthParticle, HitLocation, FRotator::ZeroRotator,FVector(0.5,0.5,0.5));
	DamagePlayMontage(index);

	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

	if (NewHealth<=0){
		//停止AI行为
		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC){
			AIC->GetBrainComponent()->StopLogic("Killed");
		}
		if (DealthParticle) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DealthParticle, GetActorLocation(), FRotator::ZeroRotator);
			//DealthParticle
			//GetWorld().D
			//UGameplayStatics::
		}
		if (DealthSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), DealthSound);
		}
		
		Destroy();
		////启用骨骼模拟
		//GetMesh()->SetAllBodiesSimulatePhysics(true);
	    //GetMesh()->SetCollisionProfileName("Ragdoll");

		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetCharacterMovement()->DisableMovement();

		//设置消失时间
		//SetLifeSpan(10);
	}

}

void ASAICharacter::SetAttackTarget(AActor* target)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor",target);
	}
}

// Called every frame
void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ASAICharacter::DamagePlayMontage(int index)
{
	switch (index)
	{
	case 1:
		PlayAnimMontage(ForwardHitMontage);
		break;
	case 2:
		PlayAnimMontage(LeftHitMontage);
		break;
	case 3:
		PlayAnimMontage(RigthHitMontage);
		break;
	case 4:
		PlayAnimMontage(BackHitMontage);
		break;
	}

}

