// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/SceneComponent.h"
#include "Data/WeaponPartData.h"
#include "Styling/SlateBrush.h"
#include "SC_WeaponPartAttachmentPoint.generated.h"


class UImage;



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONMODULAR_API USC_WeaponPartAttachmentPoint : public USceneComponent
{
	GENERATED_BODY()

public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponPartTypeProperties WeaponPointType;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	USC_WeaponPartAttachmentPoint();

	UFUNCTION()
	virtual void UpdateStaticMeshComponent();

	// Getters
	USlateBrushAsset* GetBrushTexture() {return BrushTexture;}
	UDataTable* GetUsableTable() {return UsableTable;}
	FDataTableRowHandle GetStartingWeaponPartRow() {return StartingWeaponPartRow;}
	FWeaponPartData* GetRetrievedWeaponPartData() {return RetrievedWeaponPartData;}

	// Setters
	void SetRetrievedWeaponPartData(FWeaponPartData* NewWeaponPartData);

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USlateBrushAsset> BrushTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> UsableTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle StartingWeaponPartRow;
	FWeaponPartData* RetrievedWeaponPartData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
