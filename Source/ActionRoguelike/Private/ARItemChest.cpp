// Fill out your copyright notice in the Description page of Project Settings.


#include "ARItemChest.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AARItemChest::AARItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChestMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("ChestMesh");
	RootComponent = ChestMeshComponent;

	LidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMeshComponent->SetupAttachment(ChestMeshComponent);

	LidOpenedPitch = 110.0f;
	bIsOpened = false;
	bReplicates = true;
}

void AARItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	ToggleChestLid();
}

void AARItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void AARItemChest::ToggleChestLid()
{
	bIsOpened = !bIsOpened;
	OnRep_LidOpened();
	
}

void AARItemChest::OnRep_LidOpened()
{
	float NewLidPitch = bIsOpened? LidOpenedPitch : 0;
	LidMeshComponent->SetRelativeRotation(FRotator(NewLidPitch, 0, 0));
}

void AARItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARItemChest, bIsOpened);
}
