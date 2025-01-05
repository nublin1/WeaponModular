// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/SceneComponent.h"
#include "Data/WeaponGearData.h"
#include "Styling/SlateBrush.h"
#include "SceneMarker.generated.h"

enum class EWeaponGearPartSlot : uint8;
class UImage;



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONMODULAR_API USceneMarker : public USceneComponent
{
	GENERATED_BODY()

public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponGearPartSlot WeaponMarkerType;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	USceneMarker();

	UFUNCTION()
	virtual void UpdateStaticMeshComponent();

	// Getters
	FDataTableRowHandle GetWeaponPartRow() {return WeaponPartRow;}
	FWeaponGearData* GetRetrievedWeaponPartData() {return RetrievedWeaponPartData;}

	// Setters
	void SetRetrievedWeaponPartData(FWeaponGearData* NewWeaponPartData);

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USlateBrushAsset> BrushTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle WeaponPartRow;
	FWeaponGearData* RetrievedWeaponPartData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
