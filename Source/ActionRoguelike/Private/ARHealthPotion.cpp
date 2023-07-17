// Fill out your copyright notice in the Description page of Project Settings.


#include "ARHealthPotion.h"
#include "ARAttributeComponent.h"
#include "ARCharacter.h"
#include "ARPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

AARHealthPotion::AARHealthPotion()
{
	HealthPotionMesh = CreateDefaultSubobject<UStaticMeshComponent>("PowerUpMesh");
	HealthPotionMesh->SetupAttachment(RootComponent);
	HealthPotionMesh->SetCollisionProfileName("NoCollision");
}

void AARHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
}

FText AARHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	const UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComponent && AttributeComponent->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health");
	}

	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum"), UseCost);
}

bool AARHealthPotion::CheckInteractConditions_Implementation(APawn* InstigatorPawn)
{
	const UARAttributeComponent* HealthComponent = Cast<UARAttributeComponent>(InstigatorPawn->GetComponentByClass(UARAttributeComponent::StaticClass()));
	AARPlayerState* PlayerState = Cast<AARPlayerState>(InstigatorPawn->GetPlayerState());
	
	
	const bool HealthCheck = HealthComponent && !HealthComponent->IsFullHealth();
	
	if(PlayerState && HealthCheck)	
		return  PlayerState->SubtractCredits(UseCost);	
	
	return false;
}

void AARHealthPotion::ApplyPowerUpEffect_Implementation(APawn* InstigatorPawn)
{
	UARAttributeComponent* HealthComponent = Cast<UARAttributeComponent>(InstigatorPawn->GetComponentByClass(UARAttributeComponent::StaticClass()));

	HealthComponent->ApplyHealthChange(this, HealthToRestore);
}

#undef LOCTEXT_NAMESPACE