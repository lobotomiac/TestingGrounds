// Fill out your copyright notice in the Description page of Project Settings.

#include "NextWaypoint.h"
#include "TestingGrounds.h"
#include "AIController.h"
#include "PatrollingGuard.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	// TODO protect Pointers!
	 
	// Get Patrol Points
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();
	auto PatrollingGuard = Cast<APatrollingGuard>(ControlledPawn);
	auto PatrolPoints = PatrollingGuard->PatrolPointsCpp;

	
	
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
