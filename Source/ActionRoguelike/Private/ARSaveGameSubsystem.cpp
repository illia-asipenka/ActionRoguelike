// Fill out your copyright notice in the Description page of Project Settings.


#include "ARSaveGameSubsystem.h"

#include "ARGameplayInterface.h"
#include "ARPlayerState.h"
#include "ARSaveGame.h"
#include "ARSaveGameSettings.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void UARSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UARSaveGameSettings* SGSettings = GetDefault<UARSaveGameSettings>();

	CurrentSlotName = SGSettings->SaveSlotName;

	UDataTable* DummyTable = SGSettings->DummyTablePath.LoadSynchronous();
}

void UARSaveGameSubsystem::HandleStartingNewPlayer(AController* NewPlayer)
{
	AARPlayerState* PS = NewPlayer->GetPlayerState<AARPlayerState>();
	if (ensure(PS))
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}

void UARSaveGameSubsystem::SetSlotName(FString NewSlotName)
{
	if (NewSlotName.Len() == 0)
	{
		return;
	}

	CurrentSlotName = NewSlotName;
}

void UARSaveGameSubsystem::WriteSaveGame()
{
	CurrentSaveGame->SavedActors.Empty();
	CurrentSaveGame->SavedActors.Empty();
	
	AGameStateBase* GS = GetWorld()->GetGameState();

	if (GS == nullptr)
	{
		return;
	}
	
	for (int32 i = 0; i < GS->PlayerArray.Num(); i++)
	{
		AARPlayerState* PS = Cast<AARPlayerState>(GS->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // single player only at this point
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		//Only interested in our 'gameplay actors'
		if(!Actor->Implements<UARGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Convert Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}	
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0);

	OnSaveGameWritten.Broadcast(CurrentSaveGame);
}

void UARSaveGameSubsystem::LoadSaveGame(FString InSlotName)
{
	SetSlotName(InSlotName);
	
	if(UGameplayStatics::DoesSaveGameExist(CurrentSlotName, 0))
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));

		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame Data."));

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			//Only interested in our 'gameplay actors'
			if(!Actor->Implements<UARGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if(ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);

					IARGameplayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}
		}

		OnSaveGameLoaded.Broadcast(CurrentSaveGame);
	}
	else
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::CreateSaveGameObject(UARSaveGame::StaticClass()));

		UE_LOG(LogTemp, Warning, TEXT("Created New SaveGame Data."));
	}
}


