// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmoBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

