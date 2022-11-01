// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "ARMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARMagicProjectile : public AARProjectileBase
{
	GENERATED_BODY()

public:
	
	AARMagicProjectile();

protected:
	
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ProjectileSpeed = 500.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	URadialForceComponent* RadialForceComponent;

	UFUNCTION(BlueprintCallable)
	void ApplyForce();
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	
	virtual void Tick(float DeltaTime) override;
};
