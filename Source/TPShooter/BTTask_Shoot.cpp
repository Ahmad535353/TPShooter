// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "ShooterAIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    AShooterAIController* OwnerAIController = Cast <AShooterAIController>(OwnerComp.GetAIOwner());
    AShooterCharacter* OwnerAIPawn = Cast <AShooterCharacter>(OwnerAIController->GetPawn());
    
    if (OwnerAIPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    
    OwnerAIPawn->Shoot();

    return EBTNodeResult::Succeeded;
}

