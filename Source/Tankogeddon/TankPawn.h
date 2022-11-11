// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UniPawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public AUniPawn
{
	GENERATED_BODY()

public:
	ATankPawn();

	virtual void Fire() const override; 

	void ChangeCannon();
	ACannon* getCannon(); 
	void FireSpecial();

	void MoveForward(float Value);
	void MoveRight(float Value); 
	void RotateRight(float Value); 

protected:
	virtual void BeginPlay() override;
	virtual void SetupCannon(TSubclassOf<ACannon>)override; 
	virtual void Die()override; 
	virtual void Tick(float DeltaTime) override;

	void Move(float DeltaTime); 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> EquippedCannonClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> SecondCannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Rotation")
	float TurretRotationInterpolationKey = 0.5f;

	UPROPERTY()
	class ATankPlayerController* TankController; 

private:
	float targetForwardAxisValue = 0.0f;
	float targetRightAxisValue = 0.0f; 
	float targetRotateRightAxisValue = 0.0f; 

};
