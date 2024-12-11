// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Components/TimelineComponent.h"
#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class USInterActionComponent;
class UAnimMontage;
class USAttributeComponent;
class AMyActor;
class USActionComponent;
class UInputMappingContext;
class UBoxComponent;
class UNiagaraSystem;
class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCD1,int,value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCD2,int,value);
UCLASS()
class UE_ARPG_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly,Category="Particle")
	UParticleSystemComponent* ParticleSystemComponent1;
	UPROPERTY(EditDefaultsOnly, Category = "Particle")
	UParticleSystem* ParticleSystem;
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UBoxComponent* CollisionBox;
	UPROPERTY(EditDefaultsOnly,Category="Niagara")
	UNiagaraSystem* NiagaraTemplate;

	UPROPERTY(EditDefaultsOnly,Category="Particle")
	UParticleSystem* DeathParticleSystem;

	//时间轴
	UPROPERTY()
	UTimelineComponent* Timeline_Sprint;

	//事件轴事件
	FOnTimelineFloatStatic TimelineEvent;

	UPROPERTY()
	UCurveFloat* CurveFloat;

	UPROPERTY(EditDefaultsOnly, Category = "AttackSound")
	USoundBase* AttackSound;

	bool bs = true;

	UFUNCTION()
	void CollisionBoxBegainOverride(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void CollisionBoxEndOverride(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

	UFUNCTION()
	void GenerateOverride();

	UFUNCTION()
	void CloseOverride();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta, FVector HitLocation);

	int CD1V = 5;
	int CD2V = 5;

protected:
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_Move;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_LookMouse;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_LookStick;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_Jump;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_Interact;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_Sprint;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Dash;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_SecondaryAttack;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_Skill;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_Capture; 
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_GameSetting;

	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Montage")
	UAnimMontage* AttackAnim;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Montage")
	UAnimMontage* BirthAnim;

	UPROPERTY()
	UUserWidget* GameSettingBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI");
	TSubclassOf<UUserWidget> GameSettingWidget;

	UPROPERTY()
	FTimerHandle TimerHandle_Sprint;
	UPROPERTY()
	FTimerHandle CD1Handle;
	UPROPERTY()
	FTimerHandle CD2Handle;
	UPROPERTY()
	TArray<FHitResult> HitResult;
	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	UAnimInstance* AnimInstance;

	float AttackTime=0;

	//时间轴委托
	//UPROPERTY(BlueprintAssignable,Category="Timeline")
	FOnTimelineVectorStatic OnTimelineVector;

	////曲线向量
	//UPROPERTY()
	UCurveVector* CurveVector;

	UFUNCTION()
	void OnTimelineUpdate(FVector vector);

	UFUNCTION()
	void DamagePlayMontage(int index);

	UPROPERTY(EditAnywhere, Category = "Hit")
	UAnimMontage* ForwardHitMontage;
	UPROPERTY(EditAnywhere, Category = "Hit")
	UAnimMontage* LeftHitMontage;
	UPROPERTY(EditAnywhere, Category = "Hit")
	UAnimMontage* RigthHitMontage;
	UPROPERTY(EditAnywhere, Category = "Hit")
	UAnimMontage* BackHitMontage;

public:
	UPROPERTY(BlueprintAssignable)
	FOnCD1 CD1;
	UPROPERTY(BlueprintAssignable)
	FOnCD2 CD2;

	int AttackCount = 0;

	bool isChargeAttack = false;
	bool isAttackPress = false;

	UPROPERTY()
	FVector BtnDirection;

	UPROPERTY(BlueprintReadOnly)
	float Yaw;
	UPROPERTY(BlueprintReadOnly)
	bool isSprint;

	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInterActionComponent* InterActionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(EditAnywhere,Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> Shake;

	UFUNCTION(Exec)
	void HealSelf(float s=100);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttackStart(const FInputActionInstance& Instance);
	void PrimaryAttackEnd(const FInputActionInstance& Instance);
	void PrimaryInteract();
	void PrimaryAttack_TimeElapsed();
	void Death(bool b);
	void SprintStart(const FInputActionInstance& Instance);
	void SprintStop();
	void SkillAttack();
	void Move(const FInputActionInstance& Instance);
	void LookMouse(const FInputActionValue& InputValue);
	void LookStick(const FInputActionValue& InputValue);
	void Jump();
	
	UFUNCTION()
	void SprintTimer();
	
	UFUNCTION()
	void SetCD1Value();
	UFUNCTION()
	void SetCD2Value();


	int SprintCount;

	TArray<FVector> GetAttackTracePoint();

	//抓取技能
	void CaptureStart();
	void CaptureEnd();

	UFUNCTION()
	void GameSetting();

	UFUNCTION()
	void ClearTimerHandle();
	
	virtual FVector GetPawnViewLocation() const override;
	//virtual FRotator GetViewRotation() const override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USActionComponent* ActionComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage,bool b);

	UFUNCTION()
	void OnMontageNotigy(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void OpenMontageNotify(UAnimMontage* Montage,int isOpen);
	private:
		bool bHasPawnTarget;

};
