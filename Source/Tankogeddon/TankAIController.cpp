// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TankPawn)
		Initialize();

	if (!TankPawn)
		return;

	TankPawn->MoveForward(1.0f);

	TankPawn->RotateRight(GetRotationValue());

	Targeting();
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ATankAIController::SwapWeapon()
{
	if(TankPawn)
		TankPawn->ChangeCannon();
}

float ATankAIController::GetRotationValue()
{
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
	return RotationValue;
}

void ATankAIController::Targeting()
{
	if (!IsPlayerSeen() || !IsPlayerRange())
	{
		return;
	}

	if (CanFire())
	{
		Fire();
	}
	else
	{
		RotateToPlayer();
	}
}

void ATankAIController::RotateToPlayer()
{
	TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
}

bool ATankAIController::IsPlayerRange()
{
	if (!PlayerPawn)
	{
		return false;
	}
	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;
}

bool ATankAIController::CanFire()
{
	FVector targetingDir = TankPawn->GetTurretForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	dirToPlayer.Normalize();

	float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));

	return AimAngle <= Accurency;
}

void ATankAIController::Fire()
{
	TankPawn->Fire();
}

bool ATankAIController::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = TankPawn->GetEyesPosition();
	FCollisionQueryParams params = FCollisionQueryParams();
	params.AddIgnoredActor(TankPawn);

	FHitResult hitResult;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, params))
	{
		if (hitResult.GetActor())
		{
			if (hitResult.GetActor() == PlayerPawn)
			{
				DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Green, false, 0.5f, 0, 5);
			}
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Purple, false, 0.5f, 0, 5);
			return hitResult.GetActor() == PlayerPawn;
		}
	}

	DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);
	return false;
}

void ATankAIController::Initialize()
{
TankPawn = Cast<ATankPawn>(GetPawn());
	if (!TankPawn)
	{
		return;
	}

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector pawnLocation = TankPawn->GetActorLocation();
	MovementAccurency = TankPawn->GetMovementAccurency();
	TArray<FVector> points = TankPawn->GetPatrollingPath();
	for (const FVector &point : points)
	{
		PatrollingPoints.Add(point);
	}
	CurrentPatrollingIndex = 0;
}

