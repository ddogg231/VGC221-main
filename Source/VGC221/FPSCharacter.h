// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
#include "FPSCharacter.generated.h"

// Delegate for when stats based on integers are changed.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FIntStatUpdated,
											   int32, OldValue,
											   int32, NewValue,
											   int32, MaxValue);

// Delegate for when stats based on floats are changed.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFloatStatUpdated,
											   float, OldValue,
											   float, NewValue,
											   float, MaxValue);
UCLASS()
class VGC221_API AFPSCharacter : public ACharacter
{
public:
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCameraComponent;


	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
		USkeletalMeshComponent* FPSMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category = "GamePlay")
	TSubclassOf<class AProjectile> ProjectileClass;
	
	AFPSCharacter();
	
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	virtual void Jump() override;

	virtual void Crouch(bool bClientSimulation = false) override;

	virtual void Tick(float DeltaTime) override;

	virtual  void Fire();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// The normal walking speed of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement", meta = (AllowPrivateAccess = "true"))
	float NormalMaxWalkSpeed = 400.0f;

	// The sprinting speed of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement", meta = (AllowPrivateAccess = "true"))
	float RunningMaxWalkSpeed = 800.0f;

	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void SetRunning(bool IsRunning);

	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void ToggleRunning();

	// Called to set the flag indicating the player jumped since the last update.
	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void SetHasJumped();

	// Called to se the flag indicating the player sprinted since the last update.
	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void SetHasRan();


protected:
	virtual void BeginPlay() override;

private:
	// is the character currently set to sprint?
	bool bIsRunning = false;

	// did the character sprint since the last update?
	bool bHasRan = false;

	// did the character jump since the last update?
	bool bHasJumped = false;

	// Health
	static constexpr int BaseStatValue = 100;
	float                  MaxHealth     = BaseStatValue;
	float                  CurrentHealth = BaseStatValue;
	

	GENERATED_BODY()
};
