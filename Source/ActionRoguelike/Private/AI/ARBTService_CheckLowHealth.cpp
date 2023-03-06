// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTService_CheckLowHealth.h"
#include "AIController.h"
#include "AI/ARAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();

	if(ensure(BlackBoardComp))
	{
		AARAICharacter* OwnerCharacter = Cast<AARAICharacter>(OwnerComp.GetAIOwner()->GetCharacter());
		if(ensure(OwnerCharacter))
		{
			BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, OwnerCharacter->IsLowHealth());
		}
	}
}
