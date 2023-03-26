// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTTask_HealMyself.h"
#include "AIController.h"
#include "ARAttributeComponent.h"
#include "AI/ARAICharacter.h"

EBTNodeResult::Type UARBTTask_HealMyself::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AARAICharacter* OwnerCharacter = Cast<AARAICharacter>(OwnerComp.GetAIOwner()->GetCharacter());

	if(ensure(OwnerCharacter))
	{
		UARAttributeComponent* OwnerHealthComp = Cast<UARAttributeComponent>(OwnerCharacter->GetComponentByClass(UARAttributeComponent::StaticClass()));

		if(OwnerHealthComp == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		OwnerHealthComp->ApplyHealthChange(OwnerCharacter ,HealthToRestore);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
