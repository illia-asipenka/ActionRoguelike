// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTargetDummy.h"

// Sets default values
AARTargetDummy::AARTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;

	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>("AttributeComponent");
	AttributeComp->OnHealthChanged.AddDynamic(this, &AARTargetDummy::OnHealthChanged);
}

// Called when the game starts or when spawned
void AARTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARTargetDummy::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	MeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
}

// Called every frame
void AARTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

