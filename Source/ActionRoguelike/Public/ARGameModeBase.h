// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ARGameModeBase.generated.h"

class AARPickUpCoin;
class AARHealthPotion;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

UCLASS()
class ACTIONROGUELIKE_API AARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AARGameModeBase();

public:
	virtual void StartPlay() override;

	UFUNCTION()
	virtual void OnActorKilled(AActor* Victim, AActor* Killer);

protected:
	FTimerHandle TimerHandle_SpawnBots;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	UEnvQuery* SpawnPowerUpsQuery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PowerUps")
	int CoinsForKill = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PowerUps")
	int PowerUpsToSpawn = 10;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	TSubclassOf<AARHealthPotion> HealthPotionClass;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	TSubclassOf<AARPickUpCoin> PickUpCoinClass;

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void OnPowerUpQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	void SpawnBotTimerElapsed();
	void SpawnPowerUps();

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
};
