// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"
#include"Net/UnrealNetwork.h"
#include "SGameMode.h"

bool USHealthComponent::IsFriendly(AActor* ActorA,AActor* ActorB)
{
	if (ActorA == nullptr || ActorB == nullptr) { return true; }//assume friendly
	
	USHealthComponent* HealthCompA = Cast<USHealthComponent>(ActorA->GetComponentByClass(USHealthComponent::StaticClass()));
	USHealthComponent* HealthCompB = Cast<USHealthComponent>(ActorB->GetComponentByClass(USHealthComponent::StaticClass()));

	if (HealthCompA == nullptr || HealthCompB == nullptr) { return true; }//assume friendly

	return HealthCompA->TeamNum == HealthCompB->TeamNum;
}

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{

	// ...
	DefaultHealth = 100;
	SetIsReplicated(true);
	bIsDead = false;
	TeamNum = 255;
}


void USHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || Health <= 0.0f) { return; }
	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health,-HealAmount, nullptr,nullptr,nullptr);
}

float USHealthComponent::GetHealth() const 
{
	return Health;
}

// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only hook if we are server
	if (GetOwnerRole() == ROLE_Authority) {
		AActor* MyOwner = GetOwner();
		if (MyOwner) {
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
		}
	}

	Health = DefaultHealth;
}

void USHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = Health - OldHealth;
	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

void USHealthComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f || bIsDead) { return;}

	if (DamageCauser !=DamagedActor && IsFriendly(DamagedActor, DamageCauser)) { return; }

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	bIsDead = Health <= 0.0f;//comparison

	OnHealthChanged.Broadcast(this, Health,Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead) {
		ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
		if (GM) {
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser,InstigatedBy);
		}
	}

}
void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USHealthComponent, Health);
}



