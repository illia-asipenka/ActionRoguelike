// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerController.h"

void AARPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}
