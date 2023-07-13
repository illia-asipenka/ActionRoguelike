// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAction.h"
#include "ARActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void UARAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UARActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	Comp->OnActionStarted.Broadcast(Comp, this);
}

void UARAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	UARActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
	
	Comp->OnActionStopped.Broadcast(Comp, this);
}

UWorld* UARAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

UARActionComponent* UARAction::GetOwningComponent() const
{
	return ActionComponent;
}

bool UARAction::CanStart_Implementation(AActor* Instigator)
{
	if(RepData.bIsRunning)
	{
		return false;
	}
	
	UARActionComponent* Comp = GetOwningComponent();

	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	return true;
}

void UARAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UARAction::Initialize(UARActionComponent* NewActionComponent)
{
	ActionComponent = NewActionComponent;
}

bool UARAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UARAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAction, RepData);
	DOREPLIFETIME(UARAction, ActionComponent);
	DOREPLIFETIME(UARAction, TimeStarted);
}
