// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "FPSCharacter.h"
#include "PickupUserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"


// Sets default values
AItem::AItem():ItemName(FString("Default")), ItemCount(0), ItemRarity(EItemRarity::EIR_Common), ItemState(EItemState::EIR_PickUp)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set up Mesh
	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	//Set Up collision Box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(ItemMesh);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // ignore all collision
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // added collision to viewport line only and block it

	// Create Pickup Widget above item
	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickUpWidget->SetupAttachment(GetRootComponent());

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent());
	AreaSphere->SetSphereRadius(150.f);

	// UpdatePickupText();
}


// void AItem::UpdatePickupText()
// {
// 	if (PickupWidgetRef)
// 	{
// 		// Get the desired text from the weapon and set it on the pickup widget
// 		PickupWidgetRef->ItemName(FText::FromString(ItemName));
// 	}
// }

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	// hide pickup Widget
	if(PickUpWidget)
	{
	PickUpWidget->SetVisibility(false);
	}

	SetActvieStars();
	// set up Overlap for Sphere
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverLap);


	SetItemProperties(ItemState);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(OtherActor);
		if(FPSCharacter)
		{
			FPSCharacter->IncrementOverlappedItemCount(1);
		}
	}
}

void AItem::OnSphereEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(OtherActor);
		if(FPSCharacter)
		{
			FPSCharacter->IncrementOverlappedItemCount(-1);
		}
	}
}

void AItem::SetActvieStars()
{
	for (int32 i = 0; i<= 5; i++)
	{
		ActiveStars.Add(false);
	}

	switch (ItemRarity)
	{
	case EItemRarity::EIR_Damaged:
		ActiveStars[1] = true;
		break;
		
	case  EItemRarity::EIR_Common:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		break;

	case  EItemRarity::EIR_UnCommon:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		break;


	case  EItemRarity::EIR_Rare:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		ActiveStars[4] = true;
		break;

	case  EItemRarity::EIR_Legendary:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		ActiveStars[4] = true;
		ActiveStars[5] = true;
		break;
	}
	
}

void AItem::SetItemProperties(EItemState State)
{
	switch (State)
	{
	case EItemState::EIR_PickUp:

		//set ItemMesh properties 
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//set AreaSphere properties 
		AreaSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);


		// set collision box properties 
		CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		CollisionBox->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;

	case EItemState::EIR_Equipped:
		//set ItemMesh properties 
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//set AreaSphere properties 
		AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// set collision box properties 
		CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;


	// case EItemState::EIR_PickedUP:
	// 	
	// 	break;
	//
	//
	// case EItemState::EIR_Falling:
	// 	
	// 	break;
	//
	//
	// case EItemState::EIR_EquipInterping:
	// 	
	// 	break;
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::SetItemState(EItemState State)
{
	ItemState = State;
	SetItemProperties(State);
}





