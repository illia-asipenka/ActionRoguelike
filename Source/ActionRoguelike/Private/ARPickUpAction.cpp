// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPickUpAction.h"
#include "ARActionComponent.h"


// Sets default values
AARPickUpAction::AARPickUpAction()
{
	ActionMesh = CreateDefaultSubobject<UStaticMeshComponent>("ActionMesh");
	ActionMesh->SetupAttachment(RootComponent);
	ActionMesh->SetCollisionProfileName("NoCollision"); 
}

bool AARPickUpAction::CheckInteractConditions_Implementation(APawn* InstigatorPawn)
{
	UARActionComponent* InstigatorActionComp = Cast<UARActionComponent>(InstigatorPawn->GetComponentByClass(UARActionComponent::StaticClass()));
	if(InstigatorActionComp)
	{
		UE_LOG(LogTemp, Display, TEXT("Check conditions - got component!"));
		return !InstigatorActionComp->HasAction(GrantedAction);
	}
	
	return false;
}

void AARPickUpAction::ApplyPowerUpEffect_Implementation(APawn* InstigatorPawn)
{
	UARActionComponent* InstigatorActionComp = Cast<UARActionComponent>(InstigatorPawn->GetComponentByClass(UARActionComponent::StaticClass()));
	
	if(InstigatorActionComp)
	{
		UE_LOG(LogTemp, Display, TEXT("Apply Effect - got component!"));
		InstigatorActionComp->AddAction(InstigatorPawn, GrantedAction);
	}
}


