// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerState.h"
#include "ARSaveGame.h"
#include "Net/UnrealNetwork.h"

int AARPlayerState::GetCreditsAmount() const
{
	return Credits;
}

void AARPlayerState::AddCredits(int32 AmountToAdd)
{
	if(!ensure(AmountToAdd > 0))
	{
		return;
	}
	
	Credits += AmountToAdd;
	
	if(HasAuthority())
	{
		OnCreditsChanged.Broadcast(this, Credits, AmountToAdd); 
	}
}

bool AARPlayerState::SubtractCredits(int32 AmountToSubtract)
{
	if(AmountToSubtract < 0)
	{
		return false;
	}
	
	const int NewCredits = Credits - AmountToSubtract;
	
	if(NewCredits < 0)
	{
		return false;
	}
	
	Credits = NewCredits;
	
	if(HasAuthority())
	{
		OnCreditsChanged.Broadcast(this, Credits, -AmountToSubtract);
	}
	
	return true;
}

void AARPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void AARPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPlayerState, Credits);
}

void AARPlayerState::SavePlayerState_Implementation(UARSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void AARPlayerState::LoadPlayerState_Implementation(UARSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}