// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

}

void ACannon::Fire()
{
	if (!IsReadyToFire())
	{
		return;
	}

	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, "Fire projectile");

		//if we want using VA-parameters
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("AMMo: %d"), Ammo));
	}
	else //CannonType == ECannonType::FireTrace
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire trace");
	}
	
	Ammo--;
	bReadyToFire = false;

	if (Ammo < 1)
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate * 2, false);
	}
	else
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);

}

void ACannon::FireSpecial()
{
	if (!IsReadyToFire())
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "Special fire");

	Ammo--;
	bReadyToFire = false;

	if (Ammo < 1)
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate * 2, false);
	}
	else
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
}

bool ACannon::IsReadyToFire()
{
	return bReadyToFire;
}

void ACannon::ReloadCartridge()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Out of ammo, reloading..");

	Ammo = 3;

	bReadyToFire = true;
}

void ACannon::Reload()
{
	if (!IsReadyToFire() && Ammo < 1)
		ReloadCartridge();
	else
		bReadyToFire = true;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	bReadyToFire = true;
	
}

