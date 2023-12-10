// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Task_GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class VGC221_API UTask_GetRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()


private: 
	FVector RandomLocation;
	class UNavigationSystemV1* NavArea;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
