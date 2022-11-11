// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
#include "GameStruct.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	void TakeDamage(FDamageData DamageData);
	void GetHealth() const;

	float GetHealthState() const;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BluprintReadWrite, Category = "Components")
	float MaxHealth = 10.0f;

	UPROPERTY()
	float CurrentHealth;

	void Current


};
