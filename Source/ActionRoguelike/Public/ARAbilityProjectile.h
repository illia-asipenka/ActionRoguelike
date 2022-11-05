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

private:	
	FTimerHandle LifespanTimerHandle;
	FTimerHandle TeleportationTimerHandle;
	float LifespanSeconds;
	float TeleportationDelaySeconds;
	
	UFUNCTION()
	void SetProjectileLifeSpan();	
	UFUNCTION()
	void TeleportPlayer();

	virtual void Explode_Implementation() override;
};