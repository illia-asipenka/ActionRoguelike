// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameModeBase.h"

#include "ARActionComponent.h"
#include "ARAttributeComponent.h"
#include "ARCharacter.h"
#include "ARGameplayInterface.h"
#include "ARMonsterData.h"
#include "ARPlayerState.h"
#include "ARSaveGame.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "AI/ARAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT(""), ECVF_Cheat);

AARGameModeBase::AARGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	SlotName = "SaveGame01";
}

void AARGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
	SpawnPowerUps();
}

void AARGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	AARPlayerState* PS = NewPlayer->GetPlayerState<AARPlayerState>();
	if (ensure(PS))
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void AARGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, 
                                       EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}	
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);
	
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];


			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				LogOnScreen(this, "Loading monster...", FColor::Green);
				
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AARGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}			
	
			DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
		}		
	}
}

void AARGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading.", FColor::Green);

	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		UARMonsterData* MonsterData = Cast<UARMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
	
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));
	
				UARActionComponent* ActionComponent = Cast<UARActionComponent>(NewBot->GetComponentByClass(UARActionComponent::StaticClass()));
				if (ActionComponent)
				{
					for (TSubclassOf<UARAction> ActionClass : MonsterData->Actions)
					{
						ActionComponent->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
	}	
}

void AARGameModeBase::OnPowerUpQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn powerups EQS Query Failed!"));
		return;
	}	
	
	TArray<FVector> Locations;
	QueryInstance->GetQueryResultsAsLocations(Locations);

	const int MaxLoops = Locations.Num();

	if (MaxLoops <= 0)
	{
		return;
	}
	
	for (int i = 0; i < MaxLoops; ++i)
	{
		float Chance = FMath::FRand();
		if(Chance < 0.10f)
		{
			const int32 RandomClassIndex = FMath::RandRange(0, PowerUpClasses.Num() - 1);
			TSubclassOf<AActor> RandomPowerUpClass = PowerUpClasses[RandomClassIndex];
			
			GetWorld()->SpawnActor<AActor>(RandomPowerUpClass, FVector(Locations[i].X, Locations[i].Y, 70.f), FRotator::ZeroRotator);
		}
	}	
}

void AARGameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via char 'CVarSpawnBots'."));
		return;
	}
	
	int32 NrOfAliveBots = 0;
	for(TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		AARAICharacter* Bot = *It;
		const UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.0f;
	if(DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if(NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum capacity, skipping bot spawn"));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARGameModeBase::OnQueryCompleted);
	}
}

void AARGameModeBase::SpawnPowerUps()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerUpsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARGameModeBase::OnPowerUpQueryComplete);
	}
}

void AARGameModeBase::KillAll()
{
	for(TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		AARAICharacter* Bot = *It;
		UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this); //@fixme: pass in Player for kill credits?
		}
	}
}

void AARGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	AARCharacter* Player = Cast<AARCharacter>(Victim);

	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate OnPlayerKilled;
		OnPlayerKilled.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 1.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, OnPlayerKilled, RespawnDelay, false);
	}

	Player = Cast<AARCharacter>(Killer);
	if(Player)
	{
		AARPlayerState* PlayerState = Cast<AARPlayerState>(Player->GetPlayerState());

		if(PlayerState)
		{
			PlayerState->AddCredits(CoinsForKill);
		}
	}


	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
}

void AARGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(ensure(Controller))
	{
		Controller->UnPossess();

		UE_LOG(LogTemp, Log, TEXT("About to restart player"));
		
		RestartPlayer(Controller);		
	}
}

void AARGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AARPlayerState* PS = Cast<AARPlayerState>(GameState->PlayerArray[i]);
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
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AARGameModeBase::LoadSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

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
	}
	else
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::CreateSaveGameObject(UARSaveGame::StaticClass()));

		UE_LOG(LogTemp, Warning, TEXT("Created New SaveGame Data."));
	}
}
