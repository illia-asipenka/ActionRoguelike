// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateReceived;

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	virtual void OnRep_PlayerState() override;
	virtual void SetPawn(APawn* InPawn) override;
};
