// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "../Public/SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFrameWork/PawnMovementComponent.h"
#include "SWeapon.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//adding a spring arm for the third person camera so that it moves with the(eye's of the) object instead of around it
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;//to able crouching

	//adding a third person view camera code +the header code
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp -> SetupAttachment(SpringArmComp);

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20.0f;

	WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultFOV = CameraComp->FieldOfView;

	//Spawn default weapon(rifle)
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon=GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector:: ZeroVector, FRotator::ZeroRotator);
	if (CurrentWeapon) {
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}

}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}


void ASCharacter::BeginZoom()
{
	bWantToZoom = true;
}

void ASCharacter::EndZoom()
{
	bWantToZoom = false;
}

void ASCharacter::Fire()
{
	if(CurrentWeapon){
	CurrentWeapon->Fire();
	}
}

void ASCharacter::StopFire()
{
	if (CurrentWeapon) { CurrentWeapon->StopFire(); }
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantToZoom ? ZoomedFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);//make the camere move gradually

	CameraComp->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//code for moving
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);
	
	PlayerInputComponent->BindAction("Crouch",IE_Pressed ,this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch",IE_Released, this, &ASCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopFire);

}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComp) {
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector()*Value);
}

void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector()*Value);
}

