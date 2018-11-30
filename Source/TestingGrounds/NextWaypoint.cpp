// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "NextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(IndexKey.SelectedKeyName); // combined line with this one 
	UE_LOG(LogTemp, Warning, TEXT("Waypoint Index: %i"), Index)
	
	return EBTNodeResult::Succeeded;

}
