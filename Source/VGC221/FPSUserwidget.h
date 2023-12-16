// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
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

	UFUNCTION()
	void SetHealthBar(float currentHealth, float MaxHealth);
	
	UFUNCTION()
	void SetCurrentAmmoCount(int currentAmmo);

	UFUNCTION()
	void SetReservedAmmoCount(int reservedAmmo);
	
	UPROPERTY(EditAnywhere ,meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentAmmoCounter;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ResevedAmmoCounter;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* CrossHairTop;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* CrossHairRight;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* CrossHairLeft;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* CrossHairBottom;
	
};
