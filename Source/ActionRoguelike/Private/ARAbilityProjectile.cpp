#include "ARAbilityProjectile.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AARAbilityProjectile::AARAbilityProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	LifespanSeconds = 0.5f;
	TeleportationDelaySeconds = 0.25f;
}

void AARAbilityProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetProjectileLifeSpan();
}

void AARAbilityProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentHit.AddDynamic(this, &AARAbilityProjectile::OnHitAction);
}

void AARAbilityProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARAbilityProjectile::SetProjectileLifeSpan()
{
	GetWorldTimerManager().SetTimer(LifespanTimerHandle, this, &AARAbilityProjectile::ExplodeProjectile, LifespanSeconds);
}

void AARAbilityProjectile::ExplodeProjectile()
{
	ProjectileMovementComponent->StopMovementImmediately();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, GetActorLocation());

	GetWorldTimerManager().SetTimer(TeleportationTimerHandle, this, &AARAbilityProjectile::TeleportPlayer, TeleportationDelaySeconds);
}

void AARAbilityProjectile::OnHitAction(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	LifespanTimerHandle.Invalidate();
	ExplodeProjectile();
}


void AARAbilityProjectile::TeleportPlayer()
{
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->SetActorLocation(GetActorLocation() + FVector::UpVector * 20);

	Destroy();
}
