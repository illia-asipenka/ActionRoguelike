// Fill out your copyright notice in the Description page of Project Settings.

#include "ARMagicProjectile.h"
#include "ARAttributeComponent.h"
#include "ARCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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

void AARMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	StartSelfDestroyTimer();

	if(Cast<AARCharacter>(GetInstigator()))
	{
		UGameplayStatics::SpawnEmitterAttached(AttachedParticle,GetWorld()->GetFirstPlayerController()->GetCharacter()->GetMesh(),HandSocketName);
	}	
}

void AARMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AARMagicProjectile::OnActorOverlap);
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;	
}

void AARMagicProjectile::StartSelfDestroyTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerToSelfDestroy, this, &AARMagicProjectile::Explode, TimeToDestroy);
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
			Attribute->ApplyHealthChange(-DamageAmount);

			Explode();
		}
	}
}

void AARMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeAsset, Hit.Location, 0, 1000.0f);
	ApplyForce();
}