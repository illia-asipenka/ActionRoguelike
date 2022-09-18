// Fill out your copyright notice in the Description page of Project Settings.


#include "ARInteractionComponent.h"
#include "ARGameplayInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UARInteractionComponent::UARInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UARInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	float Radius = 30.0f;
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + EyeRotation.Vector() * 1000.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	TArray<FHitResult> Hits;	
	
	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams,Shape);
	
	FColor DebugColor = bBlockingHit ? FColor::Green : FColor::Red;

	for(FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<UARGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				IARGameplayInterface::Execute_Interact(HitActor, MyPawn);
				DrawDebugSphere(GetWorld(), Hit.Location, Radius, 32, DebugColor, false, 2, 0, 2);
				break;
			}
		}
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2, 0, 2);	
}
