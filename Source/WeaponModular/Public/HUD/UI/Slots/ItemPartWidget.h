// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponPartData.h"
#include "WeaponModular/Public/HUD/UI/BUIUserWidget.h"
#include "ItemPartWidget.generated.h"

class UButton;
class USlateBrushAsset;
enum class EWeaponGearPartType : uint8;
class USceneMarker;
/**
 * 
 */
UCLASS()
class WEAPONMODULAR_API UItemPartWidget : public UBUIUserWidget
{
	GENERATED_BODY()

	public:

	protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton> ListButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneMarker> TargetMarker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> WidgetTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponGearPartType WidgetType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USlateBrushAsset> BrushTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D PointToDraw;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void GetWeaponPartsByType();
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
