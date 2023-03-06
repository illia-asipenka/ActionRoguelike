// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"
#include "ARAttributeComponent.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/ARAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("SensingComponent");
	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>("HealthComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AARAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AARAICharacter::OnHealthChanged);
}

void AARAICharacter::OnPawnSeen(APawn* Pawn)
{
	AARAIController* AIC = Cast<AARAIController>(GetController());
	if(AIC)
	{
		UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();

		BBComponent->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted!!!", nullptr, FColor::White, 4.0f, true);
	}
}

void AARAICharacter::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta < 0)
	{
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
			GetMesh()->SetCollisionProfileName("Ragdoll");000

			//set lifespan
			SetLifeSpan(10.0f);
		}
	}
}

bool AARAICharacter::IsLowHealth()
{
	return  AttributeComp->IsLowHealth();
}
