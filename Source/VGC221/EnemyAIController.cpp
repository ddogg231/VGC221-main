// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "FPSCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	if (!PawnSensing) {
		PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyAIController::OnSeePawn);
}



void AEnemyAIController::OnSeePawn(APawn* PlayerPawn)
{
	AFPSCharacter* player = Cast<AFPSCharacter>(PlayerPawn);

	if (!player) return;

	SetCanSeePlayer(true, player);
	RunTriggerableTimer();
	
}

void AEnemyAIController::SetCanSeePlayer(bool SeePlayer, UObject* Player)
{
	UBlackboardComponent* bb = GetBlackboardComponent();
	bb->SetValueAsBool(FName("CanSeePlayer"), SeePlayer);
	if (SeePlayer) {
		bb->SetValueAsObject(FName("PlayerTarget"), Player);
	}
}

void AEnemyAIController::RunTriggerableTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);
	FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"), false, GetPawn());
	GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle, FunctionDelegate, PawnSensing->SensingInterval * 2.0f, false);

}
