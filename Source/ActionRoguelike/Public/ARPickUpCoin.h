// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPowerUpBase.h"
#include "ARPickUpCoin.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARPickUpCoin : public AARPowerUpBase
{
	GENERATED_BODY()
	
public:
	AARPickUpCoin();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* CoinMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CoinsToGet = 5;
	
	virtual bool CheckInteractConditions_Implementation(APawn* InstigatorPawn) override;
	
	virtual void ApplyPowerUpEffect_Implementation(APawn* InstigatorPawn) override;
};
