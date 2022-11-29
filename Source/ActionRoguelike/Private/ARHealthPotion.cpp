// Fill out your copyright notice in the Description page of Project Settings.


#include "ARHealthPotion.h"
#include "ARAttributeComponent.h"
#include "ARCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AARHealthPotion::AARHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>("PotionMesh");
	RootComponent = PotionMesh;
}

void AARHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	UARAttributeComponent* HealthComponent = Cast<UARAttributeComponent>(InstigatorPawn->GetComponentByClass(UARAttributeComponent::StaticClass()));

	if(HealthComponent && !HealthComponent->IsFullHealth())
	{		
		HealthComponent->ApplyHealthChange(HealthToRestore);
		bCanInteract = false;
		PotionMesh->SetVisibility(false);
		GetWorld()->GetTimerManager().SetTimer(TimerToRespawn,this, &AARHealthPotion::RespawnPotion, 10.0f);
	}
}

void AARHealthPotion::RespawnPotion()
{
	bCanInteract = true;
	PotionMesh->SetVisibility(true);
}