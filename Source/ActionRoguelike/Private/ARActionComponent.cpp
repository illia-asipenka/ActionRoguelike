// Fill out your copyright notice in the Description page of Project Settings.

#include "ARActionComponent.h"
#include "ARAction.h"


UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<UARAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner() ,ActionClass);
	}
}

void UARActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void UARActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARAction> ActionClass)
{
	if(!ensureAlways(ActionClass))
	{
		return;
	}

	UARAction* NewAction = NewObject<UARAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if(NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UARActionComponent::RemoveAction(UARAction* ActionToRemove)
{
	if(!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(ActionToRemove);
}

bool UARActionComponent::HasAction(TSubclassOf<UARAction> ActionToCheck)
{
	for(UARAction* Action : Actions)
	{
		if(Action->IsA(ActionToCheck))
		{
			return true;
		}
	}

	return false;
}

bool UARActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(UARAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}
			// Is Client?
			if(!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UARActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(UARAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}			
		}
	}

	return false;
}

void UARActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}