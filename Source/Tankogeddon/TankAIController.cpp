// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TankPawn->MoveForward(1.0f);

	FVector CurrentPoint = PatrollingPoints[CurrentPatrollingIndex];
	FVector PawnLocation = TankPawn->GetActorLocation();
	if (FVector::Distance(CurrentPoint, PawnLocation) <= MovementAccurency)
	{
		CurrentPatrollingIndex++;
		if (CurrentPatrollingIndex >= PatrollingPoints.Num())
		{
			CurrentPatrollingIndex = 0;
		}
	}

	FVector moveDirection = CurrentPoint - PawnLocation;
	moveDirection.Normalize();
	FVector forwardDirection = TankPawn->GetActorForwardVector();
	FVector rightDirection = TankPawn->GetActorRightVector();

	DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

	float forwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(forwardDirection, moveDirection)));
	float rightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(rightDirection, moveDirection)));

	float RotationValue = 0.0f;
	if (forwardAngle > 2.0f)
	{
		RotationValue = 1.0f;
	}
	if (rightAngle > 90)
	{
		RotationValue = -RotationValue;
	}
	TankPawn->RotateRight(RotationValue);

	Targeting();
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	if (!TankPawn)
	{
		return;
	}

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector pawnLocation = TankPawn->GetActorLocation();
	MovementAccurency = TankPawn->GetMovementAccurency();
	TArray<FVector> points = TankPawn->GetPatrollingPath();
	for (FVector point : points)
	{
		PatrollingPoints.Add(point + pawnLocation);
	}
	CurrentPatrollingIndex = 0;
}

float ATankAIController::GetRotationValue()
{
	return 0.0f;
}

void ATankAIController::Targeting()
{
}

void ATankAIController::RotateToPlayer()
{
}

bool ATankAIController::IsPlayerRange()
{
	return false;
}

bool ATankAIController::CanFire()
{
	return false;
}

void ATankAIController::Fire()
{
}
