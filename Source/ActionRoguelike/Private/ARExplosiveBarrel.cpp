// Fill out your copyright notice in the Description page of Project Settings.

#include "ARExplosiveBarrel.h"
#include "DrawDebugHelpers.h"

// Sets default values
AARExplosiveBarrel::AARExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
	StaticMeshComponent->SetCollisionProfileName("PhysicsActor");
	StaticMeshComponent->SetSimulatePhysics(true);
	RootComponent = StaticMeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("ExplosionForceComponent");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	RadialForceComponent->Radius = 600.0f;
	RadialForceComponent->ImpulseStrength = 1000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AARExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AARExplosiveBarrel::Explode);
}

// Called every frame
void AARExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARExplosiveBarrel::Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrrel"));
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, At game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
	FString DebugMessage = FString::Printf(TEXT("Hit impact location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, DebugMessage, nullptr, FColor::Green, 2.0f, true);
}

