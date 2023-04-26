// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerState.h"

int AARPlayerState::GetCreditsAmount() const
{
	return Credits;
}

void AARPlayerState::AddCredits(int AmountToAdd)
{
	Credits += AmountToAdd;
	OnCreditsChanged.Broadcast(Credits);
}

bool AARPlayerState::SubtractCredits(int AmountToSubtract)
{
	const int NewCredits = Credits - AmountToSubtract;
	
	if(NewCredits < 0)
	{
		return false;
	}
	
	Credits = NewCredits;
	OnCreditsChanged.Broadcast(Credits);
	
	return true;
}