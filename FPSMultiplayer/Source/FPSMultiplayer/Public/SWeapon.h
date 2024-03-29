// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

//#define SURFACE_FLESHDEFAULT	 SurfaceType1;
//define SURFACE_FLESHVULNARABLE  SurfaceType2;

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

//Contains information of a single hitscan linetrace.
USTRUCT()
struct FHitScanTrace {
	GENERATED_BODY()

public:
	UPROPERTY()
		TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;

};

UCLASS()
class FPSMULTIPLAYER_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Components")
	USkeletalMeshComponent* MeshComp;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire();

	void PlayFireEffects(FVector TraceEnd);

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem*  ImpactEffect;//default impact effect

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem*  FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem*  TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;//bullets per minute fired

	float LastFireTime;

	float TimeBetweenShots;

	FTimerHandle TimeHandle_TimeBetweenShots;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)//triggers a function every time it is used
		FHitScanTrace HitScanTrace;

	//Bullet spread in degrees
	UPROPERTY(EditDefaultsOnly, Category = "Weapon",meta=(ClampMin=0.0f))
	float BulletSpread;

	UFUNCTION()
	void OnRep_HitScanTrace();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const;
public:	
	virtual void Fire();

	void StartFire();

	void StopFire();

	
	
};
