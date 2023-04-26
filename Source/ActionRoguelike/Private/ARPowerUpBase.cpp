// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPowerUpBase.h"


// Sets default values
AARPowerUpBase::AARPowerUpBase()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComponent;
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
	TogglePowerUpVisibility(false);
	GetWorld()->GetTimerManager().SetTimer(TimerToRespawn,this, &AARPowerUpBase::ShowPowerUp, RespawnSeconds);
}

void AARPowerUpBase::TogglePowerUpVisibility(bool NewVisibility)
{
	SetActorEnableCollision(NewVisibility);
	RootComponent->SetVisibility(NewVisibility, true);
}

void AARPowerUpBase::ShowPowerUp()
{
	TogglePowerUpVisibility(true);
}
