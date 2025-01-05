// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemPartIconWidget.h"
#include "Data/WeaponGearData.h"
#include "WeaponPartListWidget.h"
#include "WeaponModular/Public/HUD/UI/BUIUserWidget.h"
#include "ItemPartWidget.generated.h"

class UWeaponPartListWidget;
class UScrollBox;
class UButton;
class USlateBrushAsset;
enum class EWeaponGearPartSlot : uint8;
class USceneMarker;
/**
 * 
 */
UCLASS()
class WEAPONMODULAR_API UItemPartWidget : public UBUIUserWidget
{
	GENERATED_BODY()

public:
	//====================================================================
	// FUNCTIONS
	//====================================================================

	UFUNCTION(BlueprintCallable)
	void UpdateVisual();
	
	UFUNCTION()
	TArray<FWeaponGearData> GetWeaponPartsByType();

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemPartIconWidget> MainItemIconWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton> ListButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWeaponPartListWidget> WeaponPartListWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWeaponPartListWidget> LinkedWeaponPartListWidget;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneMarker> TargetMarkerLinked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> WidgetTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponGearPartSlot WidgetType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USlateBrushAsset> BrushTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D BrushTargetPoint;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeOnInitialized() override;

	
	UFUNCTION()
	void ListButtonClick();
	UFUNCTION()
	void PartClicked(UItemPartIconWidget* ItemPartIconWidget);
	UFUNCTION()
	void CreateWeaponPartListWidget();
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
