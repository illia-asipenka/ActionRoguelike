#include "ARAbilityProjectile.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AARAbilityProjectile::AARAbilityProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	LifespanSeconds = 0.7f;
	TeleportationDelaySeconds = 0.25f;
}

void AARAbilityProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetProjectileLifeSpan();
}

void AARAbilityProjectile::SetProjectileLifeSpan()
{
	GetWorldTimerManager().SetTimer(LifespanTimerHandle, this, &AARAbilityProjectile::Explode, LifespanSeconds);
}

void AARAbilityProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(LifespanTimerHandle);
	
	ProjectileMovementComponent->StopMovementImmediately();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, GetActorLocation());

	SetActorEnableCollision(false);

	GetWorldTimerManager().SetTimer(TeleportationTimerHandle, this, &AARAbilityProjectile::TeleportPlayer, TeleportationDelaySeconds);
}


void AARAbilityProjectile::TeleportPlayer()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation() + FVector::UpVector * 20, ActorToTeleport->GetActorRotation(), true, true);
	}

	Destroy();
}
