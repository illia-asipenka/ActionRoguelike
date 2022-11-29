// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"


// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
	Health = HealthMax;
}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UARAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

bool UARAttributeComponent::ApplyHealthChange(float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	OnHealthChanged.Broadcast(GetOwner(), this, Health, Delta);

	return true;
}







