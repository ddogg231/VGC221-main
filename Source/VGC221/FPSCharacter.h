// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
#include "Sound/SoundCue.h"
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
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaTime) override;
	
#pragma  region Gunfire handles
	virtual void Fire();

	virtual void StopFiring();

	void StartFireTimer();

	void FireWeapon();
	
	virtual void DropCurrentItem();
	
	UFUNCTION()
	void AutoFireReset();

	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	virtual  void AimIn();

	virtual  void AimOut();
	
	void CalculateCrosshairSpread(float DeltaTime);
#pragma  endregion
	
	
#pragma  region  movement
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
#pragma  endregion 

#pragma  region Camera
	
	void CameraInterpZoom(float DeltaTime);
	
	// Camera Zoom Factors
	float CameraDefaultFOV;
	
	float CameraZoomedFov;

	float CameraCurrentFOV;
#pragma  endregion 
	


	int8 GetOverlappedItemsCount() const {return  OverlappedItemCount; }

	// adds/subtracts to and from overlappedItemCount and updates bShouldTraceForItems
	void IncrementOverlappedItemCount(int8 Amount);
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// trace for Items if overlapped item count is > 0
	void TraceForItems();

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult);

	class AWeapon* SpawnStartingWeapon();

	void EquipWeapon(AWeapon* weaponToEquip);

	

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class  USoundCue* FireSound;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* Impactparticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming = false;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed;

#pragma  region CrossHairs
	/** Determines the spread of the crosshairs */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;

	/** Velocity component for crosshairs spread */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor;

	/** In air component for crosshairs spread */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;

	/** Aim component for crosshairs spread */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor;

	/** Shooting component for crosshairs spread */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;
#pragma endregion 
	
	// is the character currently set to sprint?
	bool bIsRunning = false;

	// did the character sprint since the last update?
	bool bHasRan = false;

	// did the character jump since the last update?
	bool bHasJumped = false;

	// is fire preesed/ held down?
	
	bool bFireButtonPressed;

	// is the gun ready to fire?
	bool bShouldFire;

	// should we trace for Items?
	bool bshouldTraceForItems;

	// number of overlapped Items
	int8 OverlappedItemCount;

	// the current Equipped weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> StartingWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	class AItem* TraceItemLastFrame;
	
	float AutomaticFireRate;

	FTimerHandle AutoFireTimer;
	
	// Health
	static constexpr int BaseStatValue = 100;
	float                  MaxHealth     = BaseStatValue;
	float                  CurrentHealth = BaseStatValue;
	

	GENERATED_BODY()
};
