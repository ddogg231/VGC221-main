// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerControllerBB.h"
#include "FPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void APlayerControllerBB::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	// Store a reference to the Player's Pawn
	PlayerCharacter = Cast<AFPSCharacter>(aPawn);
	checkf(PlayerCharacter,
	       TEXT("APlayerControllerBBBase derived classes should only posess ACharacterBBBase derived pawns"));

	// Get a reference to the EnhancedInputComponent
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent,
	       TEXT("Unable to get reference to the EnhancedInputComponent."));

	// Get the local player subsystem
	// Just a local variable, we dont need to refer to it again after this
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(InputSubsystem,
	       TEXT("Unable to get reference to the EnhancedInputLocalPlayerSubsystem."));

	// Wipe existing mappings, and add our mapping.
	checkf(InputMappingContext, TEXT("InputMappingContent was not specified."));
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);

	// Bind the input actions.
	// Only attempt to bind if valid values were provided.
	if (ActionIA_Move)
		EnhancedInputComponent->BindAction(ActionIA_Move, ETriggerEvent::Triggered, this,
		                                   &APlayerControllerBB::HandleMove);

	if (ActionIA_Look)
		EnhancedInputComponent->BindAction(ActionIA_Look, ETriggerEvent::Triggered, this,
		                                   &APlayerControllerBB::HandleLook);

	if (ActionIA_Jump)
		EnhancedInputComponent->BindAction(ActionIA_Jump, ETriggerEvent::Triggered, this,
		                                   &APlayerControllerBB::HandleJump);

	if (ActionIA_ToggleCrouch)
		EnhancedInputComponent->BindAction(ActionIA_ToggleCrouch, ETriggerEvent::Triggered, this,
		                                   &APlayerControllerBB::HandleToggleCrouch);

	if (ActionIA_PrimaryFire)
		EnhancedInputComponent->BindAction(ActionIA_PrimaryFire, ETriggerEvent::Triggered, this,
										   &APlayerControllerBB::HandleFire);
	
	if (ActionIA_ToggleSprint)
		EnhancedInputComponent->BindAction(ActionIA_ToggleSprint, ETriggerEvent::Triggered, this,
		                                   &APlayerControllerBB::HandleToggleSprint);
}

void APlayerControllerBB::HandleLook(const FInputActionValue& InputActionValue)
{
	// Input is a Vector2D
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	float sensitvly = 5.0f;
	// Add yaw and pitch input to controller
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}


void APlayerControllerBB::HandleMove(const FInputActionValue& InputActionValue)
{
	// Value is a Vector2D
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// Add movement to the Player's Character Pawn
	if (PlayerCharacter)
	{
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);
	}
}

void APlayerControllerBB::HandleJump()
{
	
	// Make the Player's Character Pawn jump, disabling crouch if it was active
	if (PlayerCharacter)
	{
		PlayerCharacter->UnCrouch();
		PlayerCharacter->Jump();
	}
}

void APlayerControllerBB::HandleToggleSprint()
{
	if (PlayerCharacter) PlayerCharacter->ToggleRunning();
}

void APlayerControllerBB::HandleToggleCrouch()
{
	if (PlayerCharacter && PlayerCharacter->bIsCrouched)
		PlayerCharacter->UnCrouch();
	else
		PlayerCharacter->Crouch();
}

void APlayerControllerBB::HandleFire()
{
	PlayerCharacter->Fire();
}


void APlayerControllerBB::OnUnPossess()
{
	// Unbind things here...
	EnhancedInputComponent->ClearActionBindings();

	// Call the parent method, in case it needs to do anything.
	Super::OnUnPossess();
}
