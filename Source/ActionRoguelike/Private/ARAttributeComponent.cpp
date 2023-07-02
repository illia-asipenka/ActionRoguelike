// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"

#include "ARGameModeBase.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT(""), ECVF_Cheat);


// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
	Health = HealthMax;
	Rage = 0.0f;

	SetIsReplicatedByDefault(true);
}

bool UARAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UARAttributeComponent::IsLowHealth() const
{
	return Health / HealthMax < 0.5f;
}

bool UARAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

bool UARAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged() && Delta < 0.f)
	{
		return false;
	}

	if(Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	
	const float OldHealth = Health;
	const float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);;
	const float ActualDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if(ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		if(ActualDelta < 0.0f && Health <= 0.0f)
		{
			AARGameModeBase* GM = GetWorld()->GetAuthGameMode<AARGameModeBase>();

			if(GM)
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor killed"));

				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
	return ActualDelta != 0;
}

bool UARAttributeComponent::AddRage(AActor* InstigatorActor, float HealthDelta)
{
	if(HealthDelta > 0)
	{
		return false;
	}
	
	const float RageDelta = FMath::Abs(HealthDelta) * RageConversionRate;	
	const float OldRage = Rage;

	Rage = FMath::Clamp(Rage + RageDelta, 0.0f, RageMax);
	const float ActualRageDelta = OldRage - Rage;
	
	UE_LOG(LogTemp, Warning, TEXT("Rage: %f"), Rage);

	if(ActualRageDelta == 0)
	{
		return false;
	}
	MulticastRageChanged(InstigatorActor, Rage, RageDelta);
	//OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualRageDelta);	
	return true;
}

bool UARAttributeComponent::SubtractRage(AActor* InstigatorActor, float RageDelta)
{
	if (RageDelta < 0 || RageDelta > Rage)
	{
		return false;
	}
	
	Rage -= RageDelta;
	UE_LOG(LogTemp, Warning, TEXT("Rage: %f"), Rage);
	MulticastRageChanged(InstigatorActor, Rage, RageDelta);
	//OnRageChanged.Broadcast(InstigatorActor, this, Rage, RageDelta);
	
	return true; 
}

UARAttributeComponent* UARAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<UARAttributeComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

bool UARAttributeComponent::IsActorAlive(AActor* Actor)
{
	UARAttributeComponent* AttributeComp = GetAttributes(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

void UARAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, Rage, Delta);	
}

void UARAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
}

void UARAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAttributeComponent, Health);
	DOREPLIFETIME(UARAttributeComponent, HealthMax);
	DOREPLIFETIME(UARAttributeComponent, Rage);
	DOREPLIFETIME(UARAttributeComponent, RageMax);
}
