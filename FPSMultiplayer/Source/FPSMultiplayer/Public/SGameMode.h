// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EWaveState : uint8;//forward declaration of an enum

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor,AController*,KillerController);//custom event

/**
 * 
 */
UCLASS()
class FPSMULTIPLAYER_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	FTimerHandle TimerHandle_NextWaveStart;

	FTimerHandle TimerHandle_BotSpawner;

	//Bots to spawn in current wave
	int32 NrOfBotsToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		float TimeBetweenWaves;
	
	//Hook for BP to spawn a single bot
	UFUNCTION(BlueprintImplementableEvent,Category="GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	//Start Spawning Bots
	void StartWave();

	//Stop Spawning Bots
	void EndWave();

	//Set timer for next startwave
	void PrepareForNextWave();

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void GameOver();

	void SetWaveState(EWaveState NewState);

	void RestartDeadPlayers();
public:

	ASGameMode();

	virtual void StartPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable,Category="GameModa")
	FOnActorKilled OnActorKilled;
};
