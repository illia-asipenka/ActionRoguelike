// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AARAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree, TEXT("Behavior tree is nullptr. Please assign BT in AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);	
	}
}