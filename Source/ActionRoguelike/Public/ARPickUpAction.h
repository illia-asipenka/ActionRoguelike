// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPowerUpBase.h"
#include "ARPickUpAction.generated.h"

class UARAction;

UCLASS()
class ACTIONROGUELIKE_API AARPickUpAction : public AARPowerUpBase
{
	GENERATED_BODY()

public:
	AARPickUpAction();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* ActionMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UARAction> GrantedAction;
	
	virtual bool CheckInteractConditions_Implementation(APawn* InstigatorPawn) override;
	virtual void ApplyPowerUpEffect_Implementation(APawn* InstigatorPawn) override;	
};
