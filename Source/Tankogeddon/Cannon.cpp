// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"
#include "DamageTaker.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"

// Sets default values
ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);
	CannonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootEffect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);
	ShootEffect->SetAutoActivate(false);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioEffect->SetupAttachment(SceneComp);
	AudioEffect->SetAutoActivate(false);

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
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());

		if (Projectile)
		{
			Projectile->Start();
		}
	}
	else //CannonType == ECannonType::FireTrace
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire trace");

		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams();
		traceParams.AddIgnoredActor(this);
		traceParams.bReturnPhysicalMaterial = false;

		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange;

		if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Red, false, 1.0f, 0, 5);
			if (hitResult.GetActor())
			{
				AActor* OverlappedActor = hitResult.GetActor();
				UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *OverlappedActor->GetName());

				IDamageTaker* DamagedActor = Cast<IDamageTaker>(OverlappedActor);
				if (DamagedActor)
				{
					FDamageData damageData;
					damageData.DamageValue = Damage * 2; //beam has more power
					damageData.Instigator = GetOwner();
					damageData.DamageMaker = this;
					DamagedActor->TakeDamage(damageData);
				}
				else
					OverlappedActor->Destroy(); //unessesary things
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0, 5);
		}
	}

	if (ShootEffect)
		ShootEffect->ActivateSystem();

	if (AudioEffect)
		AudioEffect->Play();

	if (CameraShake)
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraShake);
	
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

void ACannon::AddAmmo(int ammo_count)
{
	if(ammo_count > 0)
		Ammo += ammo_count;
}

int ACannon::GetAmmo()
{
	return Ammo;
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

