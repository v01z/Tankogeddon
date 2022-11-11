

#include "Turret.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Cannon.h"
#include <Kismet/KismetMathLibrary.h>
#include "HealthComponent.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	RootComponent = HitCollider;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(HitCollider);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh, "ADD_Parts_Here_Socket");

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (BodyMeshTemp)
	{
		BodyMesh->SetStaticMesh(BodyMeshTemp);
	}

	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (TurretMeshTemp)
	{
		TurretMesh->SetStaticMesh(TurretMeshTemp);
	}

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddUObject(this, &ATurret::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &ATurret::Die);

}

void ATurret::Die()
{
	Super::Die();
	Destroy();
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(CannonClass);
	
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimer;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimer, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}

void ATurret::Destroyed()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
}

void ATurret::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{

	if (!newCannonClass)
	{
		return;
	}

	FActorSpawnParameters params;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}

void ATurret::Targeting()
{
	if (!PlayerPawn)
	{
		return;
	}

	if (IsPlayerInRange())
	{
		RotateToPlayer();

		if (CanFire())
		{
			Fire();
		}
	}
}

void ATurret::RotateToPlayer()
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TargetingSpeed));
}

void ATurret::Fire()const
{
	if (Cannon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Turret fires!");//debug
		Cannon->Fire();
	}
}

bool ATurret::IsPlayerInRange()const
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire()
{
	FVector targetDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirToPlayer.Normalize();

	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetDir, DirToPlayer)));
	return aimAngle <= Accurency;
}

