// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"


UENUM(BlueprintType)
enum class EWaveState : uint8 //meanining it supports uint8 type 
{
	WaitingToStart,

	WaveInProgress,

	//No longet spawning new bots,waiting for players to kill the remaining bots
	WaitingToComplete,

	WaveComplete,

	GameOver,

};


/**
 * 
 */
UCLASS()
class FPSMULTIPLAYER_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void SetWaveState(EWaveState NewState);
	
protected:

	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);
	
	UFUNCTION(BlueprintImplementableEvent,Category="GameState")
	void WaveStateChanged(EWaveState NewState,EWaveState OldState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
		EWaveState WaveState;
};
