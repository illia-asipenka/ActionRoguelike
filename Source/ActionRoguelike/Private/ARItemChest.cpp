// Fill out your copyright notice in the Description page of Project Settings.


#include "ARItemChest.h"


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
}

// Called when the game starts or when spawned
void AARItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	ToggleChestLid();
}

void AARItemChest::ToggleChestLid()
{
	bIsOpened = !bIsOpened;

	float NewLidPitch = bIsOpened? LidOpenedPitch : 0;
	LidMeshComponent->SetRelativeRotation(FRotator(NewLidPitch, 0, 0));
}

