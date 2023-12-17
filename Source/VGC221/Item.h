// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupUserWidget.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"


UENUM(BlueprintType)
enum class EItemRarity: uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_UnCommon UMETA(DisplayName = "UnCommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_MAX UMETA(DisplayName = "DefaultMAX")
	
};
UENUM(BlueprintType)
enum class EItemState: uint8
{
	EIR_PickUp UMETA(DisplayName = "PickUp"),
	EIR_Equipped UMETA(DisplayName = "Equipped"),
	EIR_PickedUP UMETA(DisplayName = "PickedUP"),
	EIR_Falling UMETA(DisplayName = "Falling"),
	EIR_EquipInterping UMETA(DisplayName = "EquipInterping"),

	EIR_MAX UMETA(DisplayName = "DefaultMAX")
	
};
UCLASS()
class VGC221_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	

	// update the displayed text on the pickup widget
	// UFUNCTION(BlueprintCallable, Category = "Weapon")
	// void UpdatePickupText();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()// called when overlapping areaSphere
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()// called when exiting areaSphere
	void OnSphereEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetActvieStars();

//	Sets properties of the item's components based on its State
	void SetItemProperties(EItemState State);
	
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	// UPickupUserWidget* PickupWidgetRef;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	//Item mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemProerties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

	//Pick up Ui 
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickUpWidget;

	//enbales pickup radius 
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AreaSphere;
	
	//name for the items pickups
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	// Item count (ammo, weapons, medpacks, etc
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;

	// set the rarity of the Items
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	// state of the item
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "ItemProperties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;
public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return  PickUpWidget; }
	FORCEINLINE USphereComponent* GetAeraSphere() const { return AreaSphere; }
	FORCEINLINE UBoxComponent* GetCollsionBox() const { return CollisionBox; }
	FORCEINLINE EItemState GetItemState() const {return ItemState; }
	void SetItemState(EItemState State);
	FORCEINLINE USkeletalMeshComponent*  GetItemMesh() const { return ItemMesh;}
};
