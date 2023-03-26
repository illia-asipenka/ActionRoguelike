// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"
#include "ARAttributeComponent.h"
#include "ARWorldUserWidget.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/ARAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("SensingComponent");
	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>("HealthComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParamName = "TimeToHit";
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AARAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AARAICharacter::OnHealthChanged);
}

void AARAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	
	DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted!!!", nullptr, FColor::White, 0.5f, true);
}

void AARAICharacter::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta < 0)
	{

		if(InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if(ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UARWorldUserWidget>(GetWorld(), HealthBarWidgetClass);

			if(ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}	

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
		if(NewHealth <= 0)
		{
			//stop BT
			AARAIController* AIC = Cast<AARAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//set lifespan
			SetLifeSpan(10.0f);
		}
	}
}

bool AARAICharacter::IsLowHealth()
{
	return  AttributeComp->IsLowHealth();
}



void AARAICharacter::SetTargetActor(AActor* NewTarget)
{
	AARAIController* AIC = Cast<AARAIController>(GetController());
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);		
	}

}