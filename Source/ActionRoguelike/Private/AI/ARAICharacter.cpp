// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"

#include "ARActionComponent.h"
#include "ARAttributeComponent.h"
#include "ARWorldUserWidget.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/ARAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("SensingComponent");
	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>("HealthComponent");
	ActionComp = CreateDefaultSubobject<UARActionComponent>("ActionComponent");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetGenerateOverlapEvents(true);

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
	AARAIController* AIC = Cast<AARAIController>(GetController());
	if(AIC)
	{
		UObject* Target = AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor");

		if(Pawn == Target)
		{
			UE_LOG(LogTemp, Display, TEXT("Already have Target!"));
			return;
		}
	}
	
	SetTargetActor(Pawn);

	MulticastAddPlayerSpottedWidget();
	
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

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			//set lifespan
			SetLifeSpan(10.0f);
		}
	}
}

void AARAICharacter::MulticastAddPlayerSpottedWidget_Implementation()
{
	UARWorldUserWidget* PlayerSpottedWidget = CreateWidget<UARWorldUserWidget>(GetWorld(), SpottedSignWidgetClass);
	PlayerSpottedWidget->AttachedActor = this;
	PlayerSpottedWidget->AddToViewport(10);
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