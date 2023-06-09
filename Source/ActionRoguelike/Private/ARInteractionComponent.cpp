// Fill out your copyright notice in the Description page of Project Settings.


#include "ARInteractionComponent.h"
#include "ARCharacter.h"
#include "ARGameplayInterface.h"
#include "ARWorldUserWidget.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDrawDebugInteractions(TEXT("su.DrawDebugInteractions"), false, TEXT(""), ECVF_Cheat);

UARInteractionComponent::UARInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;

}

// Called when the game starts
void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UARInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void UARInteractionComponent::FindBestInteractable()
{
	bool DrawDebug = CVarDrawDebugInteractions.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AARCharacter* MyOwner = Cast<AARCharacter>(GetOwner());

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = MyOwner->GetPawnViewLocation() + EyeRotation.Vector() * TraceDistance;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	TArray<FHitResult> Hits;	
	
	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, MyOwner->GetPawnViewLocation(), End, FQuat::Identity, ObjectQueryParams,Shape);
	
	FColor DebugColor = bBlockingHit ? FColor::Green : FColor::Red;

	//clear ref before trying to fill
	FocusedActor = nullptr;
	
	for(FHitResult Hit : Hits)
	{
		if(DrawDebug)
		{
			DrawDebugSphere(GetWorld(), Hit.Location, TraceRadius, 32, DebugColor, false, 2, 0, 2);
		}
		
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<UARGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if(FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UARWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if(!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
	if (DrawDebug)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2, 0, 2);	
	}
}

void UARInteractionComponent::PrimaryInteract()
{
	if(FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact");
		return;
	}
	APawn* MyPawn = Cast<APawn>(GetOwner());
	IARGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}
