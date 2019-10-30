// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class FPSMULTIPLAYER_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")//there won't be need for a base implementation code this way
	void OnPowerupStateChanged(bool bNewIsActive);

	UFUNCTION()
	void OnRep_PowerupActive();

	UPROPERTY(ReplicatedUsing=OnRep_PowerupActive)
	bool bIsPowerupActive;//keeps state of the power up

	UPROPERTY(EditDefaultsOnly,Category="Powerups")
	float PowerupInterval;//time between powerup ticks

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalNrofTicks;//total time we apply the powerup effect

	FTimerHandle TimerHandle_PowerupTick;

	UFUNCTION()
		void OnTickPowerup();

	int32 TicksProcessed;//total number of ticks applied
public:	

	void ActivatedPowerup(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent,Category="Powerups")
	void OnActivated(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnExpired();

	
	
};
