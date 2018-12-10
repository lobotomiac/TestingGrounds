// Fill out your copyright notice in the Description page of Project Settings.

#include "NextWaypoint.h"
#include "TestingGrounds.h"
#include "AIController.h"
#include "PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// TODO protect against no PatrolRoute Component!
	// TODO protect against null Pointers!
	 
	// Get PatrolRoute

	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();

	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing Patrol Points!"), *ControlledPawn->GetName())
		if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }
	}
	
	
	// SetNextWaypoint
	//auto NumberOfPatrolPoints = PatrolPoints.Num();
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName); // combined above line with this one 

	if (PatrolPoints[Index])
	{
		BlackboardComponent->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);


		// Cycle the index
		int32 NextIndex = (Index + 1) % PatrolPoints.Num();
		BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	}
	
	return EBTNodeResult::Succeeded;

}
