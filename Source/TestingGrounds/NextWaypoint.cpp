// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "NextWaypoint.h"

EBTNodeResult::Type UNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("I'm working!"))
	
	return EBTNodeResult::Succeeded;

}
