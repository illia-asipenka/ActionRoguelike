// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameModeBase.h"
#include "ARAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "AI/ARAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"

AARGameModeBase::AARGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
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

void AARGameModeBase::SpawnBotTimerElapsed()
{
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
