// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSUserwidget.h"

void UFPSUserwidget::NativeConstruct()
{
	

}

void UFPSUserwidget::SetHealthBar(float currentHealth, float maxHealth)
{
	if(!HealthBar) return;

	HealthBar->SetPercent(currentHealth / maxHealth);
}

void UFPSUserwidget::SetCurrentAmmoCount(int currentAmmo)
{
}

void UFPSUserwidget::SetReservedAmmoCount(int reservedAmmo)
{
}


