// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPickUpCoin.h"
#include "ARPlayerState.h"

AARPickUpCoin::AARPickUpCoin()
{
	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>("CoinMesh");
	CoinMesh->SetupAttachment(RootComponent);
	CoinMesh->SetCollisionProfileName("NoCollision");
}

bool AARPickUpCoin::CheckInteractConditions_Implementation(APawn* InstigatorPawn)
{
	const AARPlayerState* PlayerState = Cast<AARPlayerState>(InstigatorPawn->GetPlayerState());
	return IsValid(PlayerState);
}

void AARPickUpCoin::ApplyPowerUpEffect_Implementation(APawn* InstigatorPawn)
{
	AARPlayerState* PlayerState = Cast<AARPlayerState>(InstigatorPawn->GetPlayerState());
	PlayerState->AddCredits(CoinsToGet);
}