// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"


// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
	Health = 100.0f;
}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UARAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(GetOwner(), this, Health, Delta);

	return true;
}







