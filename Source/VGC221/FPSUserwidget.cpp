// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSUserwidget.h"

void UFPSUserwidget::NativeConstruct()
{
	SetHealthBar(1.0);
	SetScoreCounter(0);
}

void UFPSUserwidget::SetHealthBar(float percentage)
{
	if(!HealthBar) return;

	HealthBar->SetPercent(percentage);
}

void UFPSUserwidget::SetScoreCounter(int score)
{
	if(!ScoreCounter) return;

	UIscore += score;
	ScoreCounter->SetText(FText::FromString("Score: " + FString::FromInt(UIscore)));
}
