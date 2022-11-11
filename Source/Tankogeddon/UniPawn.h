// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "UniPawn.generated.h"

class UStaticMeshComponent; 
class ACannon; //
UCLASS()
class TANKOGEDDON_API AUniPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	virtual void Fire()const;
	virtual void SetupCannon(TSubclassOf<ACannon>);

	virtual void TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
	void DamageTaked(float Value);

	UFUNCTION()
	virtual void Die();

protected:
	AUniPawn();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	class UArrowComponent* CannonSetupPoint; 

	UPROPERTY()
	ACannon* Cannon; 
};
