// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARGameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ARPowerUpBase.generated.h"

UCLASS(Abstract)
class ACTIONROGUELIKE_API AARPowerUpBase : public AActor, public IARGameplayInterface
{
	GENERATED_BODY()

public:
	AARPowerUpBase();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RespawnSeconds = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bSpawnFromStart = true;
	UPROPERTY()
	FTimerHandle TimerToRespawn;
	UPROPERTY(ReplicatedUsing = OnRep_Active, BlueprintReadOnly)
	bool bActive = true;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyPowerUpEffect(APawn* InstigatorPawn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckInteractConditions(APawn* InstigatorPawn);	
	UFUNCTION()
	void SetRespawnTimer();	
	UFUNCTION()
	void TogglePowerUpVisibility(bool NewActive);
	UFUNCTION()
	void ShowPowerUp();
	UFUNCTION()
	void OnRep_Active();
};