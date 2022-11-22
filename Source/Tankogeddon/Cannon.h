// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannon();

	void Fire();

	void FireSpecial();

	bool IsReadyToFire();
	void Reload();

	void ReloadCartridge();
	void AddAmmo(int ammo_count);
	int GetAmmo();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Fire params")
	ECannonType CannonType = ECannonType::FireProjectile;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Fire params")
	TSubclassOf<class AProjectile> ProjectileClass;
	//
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Fire params")
	TSubclassOf<class APhysicsProjectile> PhysicsProjectileClass;
	//

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Fire params")
	float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Fire params")
	float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Fire params")
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Effects")
	class UParticleSystemComponent* ShootEffect;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Effects")
	class UAudioComponent* AudioEffect;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Effects")
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Ammo")
	int Ammo = 3;

	bool bReadyToFire = true;
	FTimerHandle ReloadTimer;
};
