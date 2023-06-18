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
	
	AARItemChest();	

	virtual void Interact_Implementation(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable)
	void ToggleChestLid();	

protected:

	UFUNCTION()
	void OnRep_LidOpened();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ChestMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMeshComponent;

	UPROPERTY(EditAnywhere)
	float LidOpenedPitch;

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly)
	bool bIsOpened;
};
