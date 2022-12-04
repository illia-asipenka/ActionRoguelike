// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPowerUpBase.h"


// Sets default values
AARPowerUpBase::AARPowerUpBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PowerUpMesh = CreateDefaultSubobject<UStaticMeshComponent>("PowerUpMesh");
	RootComponent = PowerUpMesh;
}

void AARPowerUpBase::BeginPlay()
{
	if(!bSpawnFromStart)
	{
		SetRespawnTimer();
	}
}

void AARPowerUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if(CheckInteractConditions(InstigatorPawn))
	{
		ApplyPowerUpEffect(InstigatorPawn);
		SetRespawnTimer();
	}
}

void AARPowerUpBase::ApplyPowerUpEffect_Implementation(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Power up effect is not implemented!"));
}

bool AARPowerUpBase::CheckInteractConditions_Implementation(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Display, TEXT("Add interaction conditions if needed"));
	return true;
}

void AARPowerUpBase::SetRespawnTimer()
{
	bCanInteract = false;
	PowerUpMesh->SetVisibility(false);
	GetWorld()->GetTimerManager().SetTimer(TimerToRespawn,this, &AARPowerUpBase::RespawnPowerUp, RespawnSeconds);
}

void AARPowerUpBase::RespawnPowerUp()
{
	bCanInteract = true;
	PowerUpMesh->SetVisibility(true);
}