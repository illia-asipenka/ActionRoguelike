// Fill out your copyright notice in the Description page of Project Settings.


#include "ARProjectileBase.h"

// Sets default values
AARProjectileBase::AARProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionProfileName("Projectile");

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	ParticleSystemComponent->SetupAttachment(SphereComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void AARProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

