// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "FPSCharacter.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBB.generated.h"

// Forward Declarations
class UEnhancedInputComponent;
class ACharacterBB;
class UInputMappingContext;
UCLASS(Abstract)
class VGC221_API APlayerControllerBB : public APlayerController
{
public:
#pragma region  Enhancedinput Setup
	//The Input Action to map to movement.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
	UInputAction* ActionIA_Move = nullptr;

	// The Input Action to map to looking around.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
	UInputAction* ActionIA_Look = nullptr;

	// The Input Action to map to jumping.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
	UInputAction* ActionIA_Jump = nullptr;

	// The Input Action to map to Crouch.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
	UInputAction* ActionIA_ToggleCrouch = nullptr;

	// The Input Action to map to Sprint.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
	UInputAction* ActionIA_ToggleSprint = nullptr;
		
	// the input Action to use Primary fire on weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Weapons")
	UInputAction* ActionIA_PrimaryFire = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Weapons")
	UInputAction* ActionIA_Aiming = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Weapons")
	UInputAction* ActionIA_DropHeldItem = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
	UInputMappingContext* InputMappingContext = nullptr;
#pragma  endregion 
	
	
protected:
	void HandleLook(const FInputActionValue& InputActionValue);
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleJump();
	void HandleToggleSprint();
	void HandleToggleCrouch();
	void HandleFirePressed();
	void HandleFireReleased();
	void HandleAimingIn();
	void HandleAimOut();
	void HandleDropItem();
	
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	
private:

	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;

	UPROPERTY()
	AFPSCharacter* PlayerCharacter = nullptr;
	
	GENERATED_BODY()

};
