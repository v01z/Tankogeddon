// Fill out your copyright notice in the Description page of Project Settings.


#include "UniPawn.h"
#include "HealthComponent.h"
#include "Cannon.h"

AUniPawn::AUniPawn():
	BodyMesh{},
	TurretMesh{},
	HealthComponent{},
	CannonSetupPoint{},
	Cannon{}
{
	PrimaryActorTick.bCanEverTick = false;

}

void AUniPawn::Fire()const {}

void AUniPawn::SetupCannon(TSubclassOf<ACannon> newCannonClass) {}

void AUniPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void AUniPawn::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Health is: %f"), HealthComponent->GetHealth());
}

void AUniPawn::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Unit destroyed!"));
}

void AUniPawn::BeginPlay()
{
	Super::BeginPlay();
}

