#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "ARAbilityProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARAbilityProjectile: public AARProjectileBase
{
	GENERATED_BODY()
	
public:
	AARAbilityProjectile();

protected:	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
public:	
	virtual void Tick(float DeltaTime) override;

private:
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* ExplodeParticle;
	
	FTimerHandle LifespanTimerHandle;
	FTimerHandle TeleportationTimerHandle;
	float LifespanSeconds;
	float TeleportationDelaySeconds;

	UFUNCTION()
	void ExplodeProjectile();

	UFUNCTION()
	void OnHitAction(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void SetProjectileLifeSpan();

	UFUNCTION()
	void TeleportPlayer();
};