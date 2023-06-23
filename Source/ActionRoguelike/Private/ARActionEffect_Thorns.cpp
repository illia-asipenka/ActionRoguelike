// Fill out your copyright notice in the Description page of Project Settings.


#include "ARActionEffect_Thorns.h"

#include "ARActionComponent.h"
#include "ARAttributeComponent.h"
#include "ARGameplayFunctionLibrary.h"

UARActionEffect_Thorns::UARActionEffect_Thorns()
{
	DamageFraction = 0.1f;
	bAutoStart = true;
}

void UARActionEffect_Thorns::ReflectDamage(AActor* DamageInstigator, UARAttributeComponent* OwningComponent, float NewHealth, float DamageReceived)
{
	AActor* Owner = GetOwningComponent()->GetOwner();

	if(DamageReceived < 0.0f && DamageInstigator != Owner)
	{
		int32 DamageFromThorns = FMath::RoundToInt(DamageReceived * DamageFraction);

		if (DamageFromThorns == 0)
		{
			return;
		}

		DamageFromThorns = FMath::Abs(DamageFromThorns);
		
		UE_LOG(LogTemp, Warning, TEXT("ThornsDamage: %d"), DamageFromThorns);

		if(ensure(Owner))
		{
			UARGameplayFunctionLibrary::ApplyDamage(Owner, DamageInstigator, DamageFromThorns);
		}
	}	
}

void UARActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UARAttributeComponent* OwnerAttributes = UARAttributeComponent::GetAttributes(Instigator);

	if(OwnerAttributes)
	{
		OwnerAttributes->OnHealthChanged.AddDynamic(this, &UARActionEffect_Thorns::ReflectDamage);
	}
	
}

void UARActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	UARAttributeComponent* OwnerAttributes = UARAttributeComponent::GetAttributes(Instigator);

	if(OwnerAttributes)
	{
		OwnerAttributes->OnHealthChanged.RemoveDynamic(this, &UARActionEffect_Thorns::ReflectDamage);
	}

	Super::StopAction_Implementation(Instigator);
}
