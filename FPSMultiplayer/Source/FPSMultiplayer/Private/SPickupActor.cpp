// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "SPowerupActor.h"
#include "TimerManager.h"

// Sets default values
ASPickupActor::ASPickupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalCompp"));
	DecalComp->SetRelativeRotation(FRotator(90, 0.0f, 0.0f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);

	CooldownDuration = 10.0f;

	SetReplicates(true);

}

// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority) {
		Respawn();
	}
}

void ASPickupActor::Respawn()
{
	if (PowerupClass == nullptr) { return; }
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PowerUpInstance = GetWorld()->SpawnActor<ASPowerupActor>(PowerupClass, GetTransform(), SpawnParams);
}

void ASPickupActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Role==ROLE_Authority && PowerUpInstance) {
		PowerUpInstance->ActivatedPowerup(OtherActor);
		PowerUpInstance = nullptr;

		//set timer to respawn
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickupActor:: Respawn, CooldownDuration);
	}
}


