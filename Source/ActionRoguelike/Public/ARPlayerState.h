// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsChanged, int, NewCreditsAmount);

UCLASS()
class ACTIONROGUELIKE_API AARPlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Credits")
	int Credits = 0;

public:
	UFUNCTION(BlueprintPure, Category = "Credits")
	int GetCreditsAmount() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int AmountToAdd);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool SubtractCredits(int AmountToSubtract);

	UPROPERTY(BlueprintAssignable, Category = "Credits")
	FOnCreditsChanged OnCreditsChanged;
	
};
