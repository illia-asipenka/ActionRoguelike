// Fill out your copyright notice in the Description page of Project Settings.

#include "ARMagicProjectile.h"

#include "ARAttributeComponent.h"

// Sets default values
AARMagicProjectile::AARMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->Radius = 50.0f;
	RadialForceComponent->ImpulseStrength = 500.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AARMagicProjectile::BeginPlay()
{
	Super::BeginPlay();	
}

void AARMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AARMagicProjectile::OnActorOverlap);
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
}

void AARMagicProjectile::ApplyForce()
{
	RadialForceComponent->FireImpulse();
}

void AARMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		UARAttributeComponent* Attribute = Cast<UARAttributeComponent>(OtherActor->GetComponentByClass(UARAttributeComponent::StaticClass()));
		if(Attribute)
		{
			Attribute->ApplyHealthChange(-20);

			Destroy();
		}
	}
}

// Called every frame
void AARMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}