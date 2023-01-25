// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPowerUpBase.h"
#include "ARHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARHealthPotion : public AARPowerUpBase
{
	GENERATED_BODY()

public:
	AARHealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* HealthPotionMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealthToRestore = 10.0f;	

	virtual bool CheckInteractConditions_Implementation(APawn* InstigatorPawn) override;
	
	virtual void ApplyPowerUpEffect_Implementation(APawn* InstigatorPawn) override;
};