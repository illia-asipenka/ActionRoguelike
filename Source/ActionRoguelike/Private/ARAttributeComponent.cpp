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

bool UARAttributeComponent::IsLowHealth() const
{
	return Health / HealthMax < 0.5f;
}

bool UARAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

bool UARAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	const float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	const float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	return true;
}

UARAttributeComponent* UARAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<UARAttributeComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

bool UARAttributeComponent::IsActorAlive(AActor* Actor)
{
	UARAttributeComponent* AttributeComp = GetAttributes(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}
