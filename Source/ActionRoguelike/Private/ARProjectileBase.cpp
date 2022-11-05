// Fill out your copyright notice in the Description page of Project Settings.


#include "ARProjectileBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AARProjectileBase::AARProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	ParticleSystemComponent->SetupAttachment(SphereComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void AARProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentHit.AddDynamic(this, &AARProjectileBase::OnActorHit);
}

void AARProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		Explode();		
	}
}

void AARProjectileBase::Explode_Implementation()
{
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticle, GetActorLocation(), GetActorRotation());

		UE_LOG(LogTemp, Warning, TEXT("Spawned particle"));

		Destroy();
	}
}

