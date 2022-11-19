// Fill out your copyright notice in the Description page of Project Settings.


#include "HW_MapLoader.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHW_MapLoader::AHW_MapLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	MapLoaderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MapLoaderMesh"));
	MapLoaderMesh->SetupAttachment(BoxCollision);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AHW_MapLoader::OnTriggerOverlapBegin);

}

void AHW_MapLoader::SetIsActivated(bool NewIsActivated)
{
	IsActivated = NewIsActivated;
}

void AHW_MapLoader::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsActivated)
		return;

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(OtherActor == PlayerPawn)
		UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
}

