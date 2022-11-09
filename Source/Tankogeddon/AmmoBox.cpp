

#include "AmmoBox.h"
#include "Cannon.h"
#include "TankPawn.h"

AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	AmmoMesh->SetupAttachment(SceneComp);
	AmmoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AmmoMesh->SetCollisionProfileName("OverlapAll");
	AmmoMesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);

}

void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn)
	{
		TankPawn->ChangeCannon();
		ACannon* cannon = TankPawn->getCannon();
		if (cannon)
		{
			cannon->AddAmmo(ammo_present);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange,
				FString::Printf(TEXT("Added %d amount of ammo.\n Total ammo count on a current cannon is: %d"), ammo_present, cannon->GetAmmo()));
		}

		Destroy();
	}
}

