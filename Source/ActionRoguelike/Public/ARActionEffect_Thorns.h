// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARActionEffect.h"
#include "ARActionEffect_Thorns.generated.h"

class UARAttributeComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARActionEffect_Thorns : public UARActionEffect
{
	GENERATED_BODY()

protected:
	UARActionEffect_Thorns();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageFraction;

	UFUNCTION(BlueprintCallable)
	void ReflectDamage(AActor* DamageInstigator, UARAttributeComponent* OwningComponent, float NewHealth, float DamageReceived);

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
};
