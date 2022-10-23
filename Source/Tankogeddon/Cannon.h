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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	ECannonType CannonType = ECannonType::FireProjectile;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Fire params")
	float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Fire params")
	float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Fire params")
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Ammo")
	int Ammo = 3;

	bool bReadyToFire = true;
	FTimerHandle ReloadTimer;
};
