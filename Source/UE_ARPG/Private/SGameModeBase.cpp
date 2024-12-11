// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AI/SAICharacter.h"
#include <SAttributeComponent.h>
#include "EngineUtils.h"
#include <SCharacter.h>

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


void ASGameModeBase::SpawnBotTimerElapsed()
{

	if (CVarSpawnBots.GetValueOnGameThread()) {
		return;	
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,SpawnBotQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnQueryCompleted);
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller)) {
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) {
        
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	int32 NrofAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It;++It) {

		ASAICharacter* Bot = *It;
		if (USAttributeComponent::isAlives(Bot)) {
			NrofAliveBots++;
		}
	}

	float MaxBotCount = 10.0f;

	if (DifficultyCurve) {
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}

	if (NrofAliveBots >= MaxBotCount) {
		return;
	}

	TArray<FVector> Location = QueryInstance->GetResultsAsLocations();
	if (Location.IsValidIndex(0)) {
		GetWorld()->SpawnActor<AActor>(MinionClass,Location[0],FRotator::ZeroRotator);
	}

}

void ASGameModeBase::OnActorKilled(AActor* VictimeActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimeActor);
	if (Player) {
	
		FTimerHandle TimeHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed",Player->Controller);
		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimeHandle_RespawnDelay,Delegate,RespawnDelay,false);
	}

}

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}


void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&ASGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);

}

