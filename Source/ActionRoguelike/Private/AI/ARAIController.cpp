// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AARAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(PlayerPawn)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	}	
}