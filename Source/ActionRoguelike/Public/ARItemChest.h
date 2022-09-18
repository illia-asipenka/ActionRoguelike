// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ARItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARItemChest : public AActor, public IARGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARItemChest();

public:
	UPROPERTY(EditAnywhere)
	float LidOpenedPitch;

	bool bIsOpened;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ChestMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMeshComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APawn* InstigatorPawn);
	
	void ToggleChestLid();
};
