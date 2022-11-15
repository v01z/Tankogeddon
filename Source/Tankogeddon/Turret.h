// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UniPawn.h"
#include "Turret.generated.h"

UCLASS()
class TANKOGEDDON_API ATurret : public AUniPawn
{
	GENERATED_BODY()
	
public:	
	ATurret();

protected:

	virtual void Fire() const override; 
	virtual void BeginPlay() override;
	virtual void SetupCannon(TSubclassOf<ACannon> newCannonClass) override; 
	virtual void Die() override; 
	virtual void Destroyed() override;

	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange()const;
	bool CanFire();
	bool IsPlayerSeen();
	FVector GetEyesPosition() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY()
	class APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingRate = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10.0f;

	
	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";
};
