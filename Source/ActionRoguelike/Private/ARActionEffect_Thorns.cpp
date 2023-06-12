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
	if(DamageReceived > 0.0f)
	{
		return;
	}
	AActor* Owner = GetOwningComponent()->GetOwner();

	if(DamageInstigator == Owner)
	{
		return;
	}
	
	float DamageFromThorns = FMath::CeilToInt(-(DamageReceived * DamageFraction));

	if (DamageFromThorns == 0)
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("ThornsDamage: %f"), DamageFromThorns);


	if(ensure(Owner))
	{
		UARGameplayFunctionLibrary::ApplyDamage(Owner, DamageInstigator, DamageFromThorns);
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
