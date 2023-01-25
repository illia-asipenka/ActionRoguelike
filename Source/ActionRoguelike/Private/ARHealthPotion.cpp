// Fill out your copyright notice in the Description page of Project Settings.


#include "ARHealthPotion.h"
#include "ARAttributeComponent.h"
#include "ARCharacter.h"


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

bool AARHealthPotion::CheckInteractConditions_Implementation(APawn* InstigatorPawn)
{
	const UARAttributeComponent* HealthComponent = Cast<UARAttributeComponent>(InstigatorPawn->GetComponentByClass(UARAttributeComponent::StaticClass()));
	const bool CheckResult = HealthComponent && !HealthComponent->IsFullHealth();
	
	return CheckResult;
}

void AARHealthPotion::ApplyPowerUpEffect_Implementation(APawn* InstigatorPawn)
{
	UARAttributeComponent* HealthComponent = Cast<UARAttributeComponent>(InstigatorPawn->GetComponentByClass(UARAttributeComponent::StaticClass()));

	HealthComponent->ApplyHealthChange(HealthToRestore);
}