// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AMapLoader::AMapLoader()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	MapLoaderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MapLoaderMesh"));
	MapLoaderMesh->SetupAttachment(BoxCollision);

}
