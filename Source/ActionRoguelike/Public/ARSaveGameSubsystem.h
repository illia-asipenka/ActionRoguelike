// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ARSaveGameSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameSignature, class UARSaveGame*, SaveObject);

UCLASS(meta=(DisplayName = "SaveGame System"))
class ACTIONROGUELIKE_API UARSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	FString CurrentSlotName;

	UPROPERTY()
	UARSaveGame* CurrentSaveGame;

public:

	void HandleStartingNewPlayer(AController* NewPlayer);

	UFUNCTION(BlueprintCallable)
	void SetSlotName(FString NewSlotName);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame(FString InSlotName = "");

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameLoaded;
	
	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameWritten;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;	
};
