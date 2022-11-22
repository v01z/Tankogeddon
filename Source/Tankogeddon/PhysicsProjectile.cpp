// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "PhysicsComponent.h"
#include "particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));
	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(RootComponent);
}

void APhysicsProjectile::Start()
{
	moveVector = GetActorForwardVector() * TrajectorySpeed;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), moveVector,
		TrajectoryMaxTime, TrajectoryTimeStep, 0.0f);
	if (bShowTrajectory)
	{
		for (FVector position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), position, 5.0f, 8, FColor::Purple, true, 1.0f, 0, 2);
		}
	}
	TrajectoryCurrentIndex = 0;
	Super::Start();
}

void APhysicsProjectile::Move()
{
	FVector CurrentMoveVector = CurrentTrajectory[TrajectoryCurrentIndex] - GetActorLocation();
	CurrentMoveVector.Normalize();
	FVector newLocation = GetActorLocation() + CurrentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(newLocation);
	if (FVector::Distance(newLocation, CurrentTrajectory[TrajectoryCurrentIndex]) <= MoveAccurency)
	{
		TrajectoryCurrentIndex++;
		if (TrajectoryCurrentIndex >= CurrentTrajectory.Num())
		{
			Destroy();
		}
		else
		{
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
				CurrentTrajectory[TrajectoryCurrentIndex]);
			SetActorRotation(newRotation);
		}
	}
}
