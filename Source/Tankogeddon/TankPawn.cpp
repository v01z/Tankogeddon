// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include <Kismet/KismetMathLibrary.h>
#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "HealthComponent.h"

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankPawn::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Die);
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	Move(DeltaTime);

	//Tank rotation
	float yawRotation = RotationSpeed * targetRotateRightAxisValue * DeltaTime;

	FRotator currentRotation = GetActorRotation();
	yawRotation += currentRotation.Yaw;
	FRotator newRotation = FRotator(0.0f, yawRotation, 0.0f);
	SetActorRotation(newRotation);

	//Turret rotation
	if (TankController)
	{
		FVector mousePos = TankController->GetMousePosition();
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos);
		FRotator TurretRotation = TurretMesh->GetComponentRotation();
		targetRotation.Pitch = TurretRotation.Pitch;
		targetRotation.Roll = TurretRotation.Roll;

		FRotator newTurretRotation = FMath::Lerp(TurretRotation, targetRotation, TurretRotationInterpolationKey);
		TurretMesh->SetWorldRotation(newTurretRotation);
	}
}

void ATankPawn::MoveForward(float Value)
{
	targetForwardAxisValue = Value;
}

void ATankPawn::MoveRight(float Value)
{
	targetRightAxisValue = Value;
}

void ATankPawn::RotateRight(float Value)
{
	targetRotateRightAxisValue = Value;
}

void ATankPawn::Move(float DeltaTime)
{
	FVector CurrentPosition = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();

	FVector movePosition = CurrentPosition + ForwardVector * MoveSpeed * targetForwardAxisValue * DeltaTime
		+ RightVector * MoveSpeed * targetRightAxisValue * DeltaTime;

	SetActorLocation(movePosition, true);
}

void ATankPawn::Fire()const
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}

}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{
	if (!newCannonClass)
	{
		return;
	}

	if (Cannon)
	{
		Cannon->Destroy();
	}

	EquippedCannonClass = newCannonClass;
	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;
	spawnParams.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(newCannonClass, spawnParams);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATankPawn::ChangeCannon()
{
	TSubclassOf<ACannon> TempCannon;
	TempCannon = EquippedCannonClass;
	EquippedCannonClass = SecondCannonClass;
	SecondCannonClass = TempCannon;
	SetupCannon(EquippedCannonClass);
}

ACannon* ATankPawn::getCannon()
{
	return Cannon;
}

void ATankPawn::Die()
{
	Super::Die();

	if (Cannon)
	{
		Cannon->Destroy();
	}
	Destroy();

}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon(EquippedCannonClass);
}

