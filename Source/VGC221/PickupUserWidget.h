// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PickupUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class VGC221_API UPickupUserWidget : public UUserWidget
{
	GENERATED_BODY()



	
public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* UpperBackground;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LowerBackground;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* AmmoType;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* AmmoAmount;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Star1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Star2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Star3;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Star4;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Star5;

	// UFUNCTION(BlueprintCallable, Category = "UI")
	// void SetDisplayedText(const FText& NewText);
};
