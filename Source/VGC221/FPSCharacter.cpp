// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

#include "Item.h"
#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Spawning Player")));
	
	UE_LOG(LogTemp, Warning, TEXT("Spawning BP_FPSCharacter"));

	if(FPSCameraComponent)
	{
		CameraDefaultFOV = FPSCameraComponent->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}

	// spawn starting weapon and have it equipped
	EquipWeapon(SpawnStartingWeapon());
	
}

void AFPSCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	
	Super::EndPlay(EndPlayReason);
}

AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//FPS camera Set up
	// Instantiate in construes 
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPerSonCamera"));
	check(FPSCameraComponent != nullptr);

	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;
	
	// camera Vars
	CameraDefaultFOV = 0.f;
	CameraZoomedFov = 40.f;
	CameraCurrentFOV = 0.f;
	ZoomInterpSpeed = 20.f;
	
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstpersonMesh"));
	check(FPSMesh != nullptr);

	FPSMesh->SetupAttachment(FPSCameraComponent);
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);

	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(0.5f);

	// weapons State Vars
	bIsAiming = false;
	//Auto Fire vars
	AutomaticFireRate = 0.1f;
	bShouldFire = true;
	bFireButtonPressed = false;

	//Item trace Vars
	bshouldTraceForItems = false;
}

#pragma region Movement
void AFPSCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
		SetRunning(false);
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);

	// set the flag to indicate if the character ran.
	if (bIsRunning) bHasRan = true;
}

void AFPSCharacter::Jump()
{
	Super::Jump();
		UnCrouch();
		Super::Jump();
		bHasJumped = true;
	
}

void AFPSCharacter::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
	SetRunning(false);
	Crouch(true);
}

void AFPSCharacter::SetRunning(bool IsRunning)
{
	bIsRunning = IsRunning;

	// Set the speed at which the player moves, based on if they are walking or running
	GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? RunningMaxWalkSpeed : NormalMaxWalkSpeed;
}

void AFPSCharacter::ToggleRunning()
{
	SetRunning(!bIsRunning);
}

void AFPSCharacter::SetHasJumped()
{
	bHasJumped = true;
}

void AFPSCharacter::SetHasRan()
{
	bHasRan = true;
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
#pragma endregion 
// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraInterpZoom(DeltaTime);
	CalculateCrosshairSpread(DeltaTime);
	TraceForItems();
}


#pragma  region Fire functions
void AFPSCharacter::Fire()
{
	
	bFireButtonPressed = true;
	FireWeapon();
}

void AFPSCharacter::StopFiring()
{
	bFireButtonPressed = false;
}

void AFPSCharacter::FireWeapon()
{
	StartFireTimer();
}
void AFPSCharacter::StartFireTimer()
{
	if(bShouldFire)
	{
		if(FireSound)
		{
			UGameplayStatics::PlaySound2D(this, FireSound);
		}
		bShouldFire = false;
		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AFPSCharacter::AutoFireReset, AutomaticFireRate);
	}
}
void AFPSCharacter::AutoFireReset()
{
	bShouldFire = true;
	if(bFireButtonPressed)
	{
		StartFireTimer();
	}
}

bool AFPSCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult,FVector& OutHitLocation)
{
	// Get Viewport Size
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Get screen space location of crosshairs
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Get world position and direction of crosshairs
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		// Trace from Crosshair world location outward
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirection * 50'000.f };
		OutHitLocation = End;
		
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_Visibility);
		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}
bool AFPSCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult)
{
	FVector OutBeamLocation;
	// check for crosshair line trace hit
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation);

	if(bCrosshairHit)
	{
		// still need line trace from gun
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else // if no cross hair line trace hit do nothing outBeamLocation = end location for line trace
	{
		
	}
	// Perform a second trace, this time from the gun barrel
	const FVector WeaponTraceStart{ MuzzleSocketLocation };
	const FVector StartToEnd{ OutBeamLocation - WeaponTraceStart };
	// ensure the second line trace extends farther then first line trace to prevent 2nd from ending early and make sure it hits its target
	const FVector WeaponTraceEnd{ MuzzleSocketLocation + StartToEnd * 1.25f }; 
	GetWorld()->LineTraceSingleByChannel(OutHitResult, WeaponTraceStart, WeaponTraceEnd, ECC_Visibility);

	if (!OutHitResult.bBlockingHit) // object between barrel and BeamEndPoint?
		{
		OutHitResult.Location = OutBeamLocation;
		return false;
		}

	return true;
}

AWeapon* AFPSCharacter::SpawnStartingWeapon()
{
	//check if Tsubclass of variable is valid
	if(StartingWeapon)
	{
			check(GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Spawned weapon")));
		return  GetWorld()->SpawnActor<AWeapon>(StartingWeapon);
	}
	return nullptr;
}

void AFPSCharacter::EquipWeapon(AWeapon* weaponToEquip)
{
	if(EquippedWeapon)
	{
	
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("Right Hand Socket"));
		if(HandSocket)
		{
			HandSocket->AttachActor(weaponToEquip,GetMesh());

		}
		EquippedWeapon = weaponToEquip;
		EquippedWeapon->SetItemState(EItemState::EIR_Equipped);
	}
}

void AFPSCharacter::DropCurrentItem()
{
	if(EquippedWeapon)
	{
		// weapon maybe modefied unexpectedly when dropped
		//Detach from FPS mesh
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);
	}
}

void AFPSCharacter::AimIn()
{
	bIsAiming = true;
}
void AFPSCharacter::AimOut()
{
	bIsAiming = false;
}


void AFPSCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange{ 0.0f, 400.0f};
	FVector2D VelocityMultiplierRange{ 0.0f, 1.0f};
	FVector Velocity{ GetVelocity() };
	Velocity.Z = 0.f;

	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());
	
	CrosshairSpreadMultiplier = 0.5F + CrosshairVelocityFactor;
}

#pragma endregion 
void AFPSCharacter::CameraInterpZoom(float DeltaTime)
{
	if(bIsAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV,CameraZoomedFov, DeltaTime, ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV,CameraDefaultFOV, DeltaTime, ZoomInterpSpeed);
	}
	FPSCameraComponent->SetFieldOfView(CameraCurrentFOV);
}

void AFPSCharacter::IncrementOverlappedItemCount(int8 Amount)
{
	if (OverlappedItemCount + Amount <= 0)
	{
		OverlappedItemCount = 0;
		bshouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bshouldTraceForItems = true;
	}
}

void AFPSCharacter::TraceForItems()
{
	if(bshouldTraceForItems)
	{
		FHitResult ItemTraceResults;
		FVector Hitlocation;
		TraceUnderCrosshairs(ItemTraceResults, Hitlocation);
		if(ItemTraceResults.bBlockingHit)
		{
			AItem* HitItem = Cast<AItem>(ItemTraceResults.GetActor());

			if(HitItem && HitItem->GetPickupWidget())
			{
				// show item's pickup widget
				HitItem->GetPickupWidget()->SetVisibility(true);
			}

			if(TraceItemLastFrame)
			{
				if (HitItem != TraceItemLastFrame)
				{
					//hitting a different item from last frame or item is null
					TraceItemLastFrame->GetPickupWidget()->SetVisibility(false);
				}
			}
			
			//store the last item hit by line trace
			TraceItemLastFrame = HitItem;
		}
	}
	else if (TraceItemLastFrame)
	{
		// no longer overlapping any items
		TraceItemLastFrame->GetPickupWidget()->SetVisibility(false);
	}
}



