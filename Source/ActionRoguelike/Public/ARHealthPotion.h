// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ARHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARHealthPotion : public AActor, public IARGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARHealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* PotionMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealthToRestore = 10.0f;

private:
	bool bCanInteract = true;
	FTimerHandle TimerToRespawn;

	UFUNCTION()
	void RespawnPotion();
};
