 // Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SAttributeComponent.h"
#include "SInterActionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyActor.h"
#include "AI/SAICharacter.h"
#include "SGameplayFunctionLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SActionComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Curves/CurveVector.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraCommon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Animation/AnimSequence.h"

// Sets default values
ASCharacter::ASCharacter(){
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	/*SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetUsingAbsoluteRotation(true);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;*/

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InterActionComp = CreateDefaultSubobject<USInterActionComponent>("InterActionComp");
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	ParticleSystemComponent1 = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem1");

	CollisionBox= CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	CollisionBox->SetupAttachment(GetMesh(),FName("Weapon_01"));
	
	////初始化时间轴
	Timeline_Sprint = CreateDefaultSubobject<UTimelineComponent>("Timeline");

	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASCharacter::HealSelf(float s)
{
	AttributeComponent->ApplyHealthChange(nullptr,s,FVector());
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->bUpdateOverlapsOnAnimationFinalize = false;
	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->OnMontageEnded.AddDynamic(this, &ASCharacter::OnMontageEnded);
	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this,&ASCharacter::OnMontageNotigy);

	AttributeComponent->OnHealthChanged.AddDynamic(this,&ASCharacter::OnHealthChanged);
	AttributeComponent->OnDeath.BindUObject(this, &ASCharacter::Death);
	AttributeComponent->SetHealth(100);

	ParticleSystemComponent1->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("HammerCenter"));
	CollisionBox->IgnoreActorWhenMoving(this, true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASCharacter::CollisionBoxBegainOverride);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASCharacter::CollisionBoxEndOverride);
	GetCapsuleComponent()->SetShouldUpdatePhysicsVolume(true);

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp) {
		MoveComp->MaxWalkSpeed = 300;
		MoveComp->MaxWalkSpeedCrouched = 150;
	}

	/*TimelineEvent.BindUFunction(this, TEXT("SprintTimer"));
	CurveFloat = NewObject<UCurveFloat>();
	CurveFloat->FloatCurve.AddKey(0,0);
	CurveFloat->FloatCurve.AddKey(0.1,0.1);
	Timeline_Sprint->AddInterpFloat(CurveFloat,TimelineEvent);
	Timeline_Sprint->SetLooping(false);
	Timeline_Sprint->SetTimelineLength(0.5f);*/
	
	PlayAnimMontage(BirthAnim);
	//OnTimelineVector.AddDynamic(this,&ASCharacter::OnTimelineUpdate);
}

void ASCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}


void ASCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector(),value);
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ShouldSkipUpdateOverlaps();
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(DefaultInputMapping,0);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	InputComp->BindAction(Input_Move,ETriggerEvent::Triggered,this,&ASCharacter::Move);
	InputComp->BindAction(Input_LookMouse,ETriggerEvent::Triggered,this,&ASCharacter::LookMouse);
	InputComp->BindAction(Input_LookStick, ETriggerEvent::Triggered,this,&ASCharacter::LookStick);
	InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttackStart);
	InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Completed, this, &ASCharacter::PrimaryAttackEnd);

	// Sprint while key is held
	InputComp->BindAction(Input_Sprint, ETriggerEvent::Triggered, this, &ASCharacter::SprintStart);
	InputComp->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ASCharacter::Jump);
	InputComp->BindAction(Input_Interact, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);
	InputComp->BindAction(Input_Skill,ETriggerEvent::Triggered,this,&ASCharacter::SkillAttack);

	InputComp->BindAction(Input_Capture,ETriggerEvent::Started,this,&ASCharacter::CaptureStart);
	InputComp->BindAction(Input_Capture,ETriggerEvent::Completed,this,&ASCharacter::CaptureEnd);

	InputComp->BindAction(Input_GameSetting,ETriggerEvent::Started,this,&ASCharacter::GameSetting);

}


#pragma region Attack

void ASCharacter::PrimaryAttackStart(const FInputActionInstance& Instance) {
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("f=%d"), RootComponent->SkipUpdateOverlapsOptimEnabled));
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("m=%d"), GetMesh()->ShouldSkipUpdateOverlaps()));

	if (!AnimInstance->Montage_IsPlaying(AttackAnim) && !isChargeAttack)
	{
		isAttackPress = true;
		float t = Instance.GetElapsedTime();
		if (t < 0.5)
			isChargeAttack = false;
		else
			isChargeAttack = true;
		UE_LOG(LogTemp, Log, TEXT("Attack1"));
		if (isChargeAttack)
		{
			ParticleSystemComponent1->Activate();
		}
		else
		{
			if (AttackCount <= 0)
			{
				PlayAnimMontage(AttackAnim);
			}
		}
	}
	AttackCount++;
}

void ASCharacter::PrimaryAttackEnd(const FInputActionInstance& Instance)
{
	if (!AnimInstance->Montage_IsPlaying(AttackAnim))
	{
		if (isChargeAttack) {
			AttackCount = 2;
			OpenMontageNotify(AttackAnim, 1);
			PlayAnimMontage(AttackAnim);
		}
	}

}

#pragma endregion


void ASCharacter::Death(bool b)
{
	if (b) {
	  

	}
}

#pragma region Movement

void ASCharacter::Move(const FInputActionInstance& Instance)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// Get value from input (combined value from WASD keys or single Gamepad stick) and convert to Vector (x,y)
	const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();
	BtnDirection = FVector(AxisValue.Y, AxisValue.X, 0.0f);
	Yaw = BtnDirection.Rotation().Yaw;
	//FString str = FString::Printf(TEXT("X:%f      Y:%f"), AxisValue.X, AxisValue.Y);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,FString::Printf(TEXT("%f"),Yaw));
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, GetActorRotation().ToString());
	AddMovementInput(ControlRot.Vector(), AxisValue.Y);
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, AxisValue.X);

}

void ASCharacter::LookMouse(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ASCharacter::LookStick(const FInputActionValue& InputValue)
{
	FString FailedMsg = FString::Printf(TEXT("Hello"));
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
	//FVector2D Value = InputValue.Get<FVector2D>();

	//// Track negative as we'll lose this during the conversion
	//bool XNegative = Value.X < 0.f;
	//bool YNegative = Value.Y < 0.f;

	//// Can further modify with 'sensitivity' settings
	//static const float LookYawRate = 100.0f;
	//static const float LookPitchRate = 50.0f;

	//// non-linear to make aiming a little easier
	//Value = Value * Value;

	//if (XNegative)
	//{
	//	Value.X *= -1.f;
	//}
	//if (YNegative)
	//{
	//	Value.Y *= -1.f;
	//}

	//// Aim assist
	//// todo: may need to ease this out and/or change strength based on distance to target
	//float RateMultiplier = 1.0f;
	//if (bHasPawnTarget)
	//{
	//	RateMultiplier = 0.5f;
	//}

	//AddControllerYawInput(Value.X * (LookYawRate * RateMultiplier) * GetWorld()->GetDeltaSeconds());
	//AddControllerPitchInput(Value.Y * (LookPitchRate * RateMultiplier) * GetWorld()->GetDeltaSeconds());
}

void ASCharacter::Jump()
{
	bPressedJump = true;
}

//void ASCharacter::SprintTimer(float f)
void ASCharacter::SprintTimer()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("%f"), 1));
	GetCharacterMovement()->Velocity = GetActorForwardVector().RotateAngleAxis(Yaw, FVector::UpVector) * 2000;
	UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraTemplate, GetMesh(), FName(TEXT("")), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	SprintCount++;
	if (SprintCount >= 4)
	{
		SprintCount = 0;
		GetWorldTimerManager().ClearTimer(TimerHandle_Sprint);
	}
}

void ASCharacter::SetCD1Value()
{
	if(CD1.IsBound()) 
	{
		if (CD1V == -1) {
			GetWorldTimerManager().ClearTimer(CD1Handle);
			CD1V = 6;
		}
		else {
			CD1.Broadcast(CD1V);
		}
		CD1V--;
	}
}
void ASCharacter::SetCD2Value()
{
	if (CD2.IsBound())
	{
		if (CD2V == -1) {
			GetWorldTimerManager().ClearTimer(CD2Handle);
			CD2V = 6;
		}
		else {
			CD2.Broadcast(CD2V);
		}
		CD2V--;
	}

}
#pragma endregion



void ASCharacter::SprintStart(const FInputActionInstance& Instance)
{
	float t = Instance.GetElapsedTime();
	if (!isSprint) 
	{
		isSprint = true;
		if (t > 1)
		{
			ActionComp->StartActionByName(this, "Sprint");
		}
		else 
		{
			GetWorldTimerManager().SetTimer(TimerHandle_Sprint, this, &ASCharacter::SprintTimer, 0.05, true, 0);
		}
	}
	else {
		isSprint = false;
		ActionComp->StopActionByName(this, "Sprint");
	}
	
}

void ASCharacter::SkillAttack()
{
	if (!CD1Handle.IsValid()) {
		GetWorldTimerManager().SetTimer(CD1Handle, this, &ASCharacter::SetCD1Value, 1, true, 0);
		ActionComp->StartActionByName(this, "SkillAttack");
	}
}

TArray<FVector> ASCharacter::GetAttackTracePoint()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Weapon_02");
	FVector EndLocation = GetMesh()->GetSocketLocation("Weapon_01");
	FVector V = (EndLocation - HandLocation) / 4;
	//FRotator Rotator = GetMesh()->GetSocketRotation("Weapon_02");
	//FVector ForwardV = UKismetMathLibrary::GetRightVector(Rotator);
	TArray<FVector> Point;
	//Point.Insert(HandLocation,0);
	//Point.Insert(EndLocation,1);
	for (int i = 0; i < 4; i++) {
		//FVector v1=ForwardV * (i *(-10));
		FVector v2=HandLocation + (V*i);
		Point.Insert(v2,i);
	}
	return Point;
}

void ASCharacter::CaptureStart()
{
	ActionComp->StartActionByName(this, "Capture");
}

void ASCharacter::CaptureEnd()
{
	if (!CD2Handle.IsValid()) {
		GetWorldTimerManager().SetTimer(CD2Handle, this, &ASCharacter::SetCD2Value, 1, true, 0);
		ActionComp->StopActionByName(this, "Capture");
	}
}

void ASCharacter::GameSetting()
{
	if (GameSettingBar == NULL) {
		GameSettingBar = CreateWidget(GetWorld(), GameSettingWidget);
	}
	if (GameSettingBar){
		if (GameSettingBar->IsInViewport())
		{
			GameSettingBar->RemoveFromViewport();
			APlayerController* Controllerd = GetController<APlayerController>();
			if (Controllerd)
			{
				Controllerd->bShowMouseCursor = false;
				Controllerd->SetIgnoreMoveInput(false);
				Controllerd->SetIgnoreLookInput(false);
				Controllerd->SetPause(false);
			}
		}	
		else 
		{
			GameSettingBar->AddToViewport();
			APlayerController* Controllerd = GetController<APlayerController>();
			if (Controllerd) {
				Controllerd->bShowMouseCursor = true;
				Controllerd->SetIgnoreMoveInput(true);
				Controllerd->SetIgnoreLookInput(true);
				Controllerd->SetPause(true);
			}
			
		}
			
	}


}

void ASCharacter::ClearTimerHandle()
{
	UE_LOG(LogTemp, Display, TEXT("ClearTimer!"));
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

//FRotator ASCharacter::GetViewRotation() const
//{
//	return CameraComp->GetComponentRotation();
//}

#pragma region MontageEvent

void ASCharacter::OnMontageEnded(UAnimMontage* Montage, bool b)
{
	if (Montage->GetName().Equals(TEXT("AM_PlayerAttack"))) {
		if (isChargeAttack) {
			OpenMontageNotify(AttackAnim, 0);
			if (!isAttackPress)
				ParticleSystemComponent1->Deactivate();
			isChargeAttack = false;
		}
		isAttackPress = false;
		
	}
	AttackCount = 0;
	CloseOverride();
}

void ASCharacter::OnMontageNotigy(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	int num=NotifyName.GetNumber();

	switch (num) {
	case 1:
		GenerateOverride();
		break;
	case 2:
		CloseOverride();
		break;
	case 3:
		{	AttackCount--;
			if (AttackCount <= 0) 
			{
				StopAnimMontage(AttackAnim);
				UE_LOG(LogTemp, Display, TEXT("MontageNotifyName!"));
			}
			else 
			{
				if (CollisionBox->MoveIgnoreActors.Num() >= 2) {
					CollisionBox->MoveIgnoreActors.RemoveAt(1, CollisionBox->MoveIgnoreActors.Num() - 1);
				}
				GenerateOverride();
			}
		}
		break;
	case 4:
		CloseOverride();
		break;
	
	}
	
}

void ASCharacter::OpenMontageNotify(UAnimMontage* Montage, int isOpen)
{
	//UAnimSequenceBase* Sq = Cast<UAnimSequenceBase>(Montage);
	if (Montage) {
		if (Montage->Notifies.Num() > 0) {
			for (int i = 0; i < Montage->Notifies.Num(); i++) {
				if (Montage->Notifies[i].NotifyName.IsEqual("PlayParticleEffect"))
					Montage->Notifies[i].NotifyTriggerChance = isOpen;
			}
		}
	}
}

#pragma endregion



void ASCharacter::PrimaryInteract() {
	if (InterActionComp) {
		InterActionComp->PrimaryInteract();
	}
}

void ASCharacter::CollisionBoxBegainOverride(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (SweepResult.GetActor() != this)
	{
		/*UCharacterMovementComponent* MoveComp = HitResult[i].GetActor()->GetComponentByClass<UCharacterMovementComponent>();
		if (MoveComp) {
			FVector V = FVector(0.0f, 0.0f, 1.0f);
			MoveComp->Launch(V * 1000);
		}*/
		CollisionBox->IgnoreActorWhenMoving(SweepResult.GetActor(), true);
		CollisionBox->IgnoreActorWhenMoving(OtherActor, true);
		float Harm = -10.0f;
		if (isChargeAttack)
			Harm *= 2;
		FVector Hit = (CollisionBox->K2_GetComponentLocation() + SweepResult.GetActor()->GetActorLocation()) / 2;
		USGameplayFunctionLibrary::ApplyDirectionalDamage(this, SweepResult.GetActor(), Harm, SweepResult, CollisionBox->K2_GetComponentLocation());
		//USGameplayFunctionLibrary::ApplyDirectionalDamage(this, SweepResult.GetActor(), Harm, SweepResult, Hit);
		UGameplayStatics::PlaySound2D(GetWorld(), AttackSound);
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		PlayerController->ClientStartCameraShake(Shake);
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), Shake, GetActorLocation(), 0, 3000);
		//AnimInstance->Montage_SetPlayRate(AttackAnim,0.1);
		/*if (ShowHarm.IsBound())
		{
			ShowHarm.Broadcast(SweepResult.Location);
		}*/

	}
	else 
	{
	     //GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, SweepResult.GetActor()->GetName());
	     //OverlappedComponent->IgnoreActorWhenMoving(SweepResult.GetActor(), true);
	}

}

void ASCharacter::CollisionBoxEndOverride(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//AnimInstance->Montage_SetPlayRate(AttackAnim,1);
}

void ASCharacter::GenerateOverride()
{
	CollisionBox->SetGenerateOverlapEvents(true);
	GetMesh()->bUpdateOverlapsOnAnimationFinalize = true;
}

void ASCharacter::CloseOverride()
{
	CollisionBox->SetGenerateOverlapEvents(false);
	GetMesh()->bUpdateOverlapsOnAnimationFinalize = false;
	if (CollisionBox->MoveIgnoreActors.Num() >= 2) {
		CollisionBox->MoveIgnoreActors.RemoveAt(1, CollisionBox->MoveIgnoreActors.Num() - 1);
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta, FVector HitLocation)
{

	if (!isAttackPress)
	{
		int index = USGameplayFunctionLibrary::AttackLocation(GetActorLocation(), GetActorRotation(), HitLocation);
		DamagePlayMontage(index);
	}

	// Died
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		GetMesh()->SetVisibility(false);
		SetLifeSpan(3.0f);
	}
}

void ASCharacter::OnTimelineUpdate(FVector vector)
{
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,vector.ToString());
}

void ASCharacter::DamagePlayMontage(int index)
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


