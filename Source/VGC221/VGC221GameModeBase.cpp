// Copyright Epic Games, Inc. All Rights Reserved.


#include "VGC221GameModeBase.h"




void AVGC221GameModeBase::StartPlay()
{
	Super::StartPlay();

	ChangeMenuWidget(StartingWidgetClass);
    
    	// Log to Screen
    	// C Assert
    	check(GEngine != nullptr)
    	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("STarting Map")));
    
    	
}

void AVGC221GameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	if(NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		CurrentWidget->AddToViewport();
	}
}
