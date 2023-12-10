// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "FPSUserwidget.generated.h"

/**
 * 
 */
UCLASS()
class VGC221_API UFPSUserwidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreCounter;
	
	UFUNCTION()
	void SetHealthBar(float percentage);

	UFUNCTION()
	void SetScoreCounter(int ammo);

private:
	int UIscore = 0;
};
