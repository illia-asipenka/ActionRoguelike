// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AARPlayerState*, PlayerState, int32, NewCreditsAmount, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API AARPlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	UPROPERTY(ReplicatedUsing = OnRep_Credits, VisibleAnywhere, Category = "Credits")
	int32 Credits = 0;

public:
	UFUNCTION(BlueprintPure, Category = "Credits")
	int GetCreditsAmount() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 AmountToAdd);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool SubtractCredits(int32 AmountToSubtract);

	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

	UPROPERTY(BlueprintAssignable, Category = "Credits")
	FOnCreditsChanged OnCreditsChanged;	
};
