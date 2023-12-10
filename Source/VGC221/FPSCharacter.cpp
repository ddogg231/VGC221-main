// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Spawning Player")));
	
	UE_LOG(LogTemp, Warning, TEXT("Spawning BP_FPSCharacter"));
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


	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstpersonMesh"));
	check(FPSMesh != nullptr);

	FPSMesh->SetupAttachment(FPSCameraComponent);
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);

	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(0.5f);
	
}

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

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFPSCharacter::Fire()
{
	
}
