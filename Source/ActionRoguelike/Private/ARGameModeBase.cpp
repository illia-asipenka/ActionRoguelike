// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameModeBase.h"
#include "ARAttributeComponent.h"
#include "ARCharacter.h"
#include "ARHealthPotion.h"
#include "ARPickUpCoin.h"
#include "ARPlayerState.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "AI/ARAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT(""), ECVF_Cheat);

AARGameModeBase::AARGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
	SpawnPowerUps();
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
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
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
		UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(Bot);
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
