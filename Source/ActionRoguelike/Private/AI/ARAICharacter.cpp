// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"
#include "DrawDebugHelpers.h"
#include "AI/ARAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("SensingComponent");
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AARAICharacter::OnPawnSeen);
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