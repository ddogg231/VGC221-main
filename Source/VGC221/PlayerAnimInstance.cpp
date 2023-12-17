// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"
#include "FPSCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(fpsCharacter == nullptr)
	{
		fpsCharacter = Cast<AFPSCharacter>(TryGetPawnOwner());
	}

	if(fpsCharacter)
	{
		// get the horazontal speed of the player from velocity
		FVector Velocity{fpsCharacter->GetVelocity()};
		Velocity.Z = 0;
		speed = Velocity.Size();

		// is the player in the Air?
		bIsInAir = fpsCharacter->GetCharacterMovement()->IsFalling();

		// is the character accelerating?
		if(fpsCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bisAccelerating = true;
		}
		else
		{
			bisAccelerating = false;
		}
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	fpsCharacter = Cast<AFPSCharacter>(TryGetPawnOwner());
}
