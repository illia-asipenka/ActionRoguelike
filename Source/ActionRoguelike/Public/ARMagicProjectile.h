// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "GameplayTagContainer.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "ARMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARMagicProjectile : public AARProjectileBase
{
	GENERATED_BODY()

public:	
	AARMagicProjectile();
	virtual void BeginPlay() override;

protected:
	virtual void PostInitializeComponents() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ProjectileSpeed = 500.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 50.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	URadialForceComponent* RadialForceComponent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UParticleSystem* AttachedParticle;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShakeAsset;

	UPROPERTY()
	FTimerHandle TimerToSelfDestroy;

	UPROPERTY(EditDefaultsOnly)
	float TimeToDestroy = 10.0f;
	
	UPROPERTY(VisibleAnywhere)
	FName HandSocketName = "Muzzle_01";

	UFUNCTION()
	void StartSelfDestroyTimer();
	
	UFUNCTION(BlueprintCallable)
	void ApplyForce();	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
