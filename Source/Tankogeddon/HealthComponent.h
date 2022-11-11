// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStruct.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_EVENT(UHealthComponent, FOnDie);
	DECLARE_EVENT_OneParam(UHealthComponent, FOnHealthChanged, float);

public:	

	void TakeDamage(FDamageData DamageData);
	float GetHealth() const;

	float GetHealthState() const;
	void AddHealth(float newHealth);

	FOnDie OnDie;
	FOnHealthChanged OnHealthChanged;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float MaxHealth = 10.0f;

	UPROPERTY()
	float CurrentHealth;

};
