// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemPartIconWidget.h"
#include "Data/WeaponPartData.h"
#include "WeaponPartListWidget.h"
#include "Slate/SlateBrushAsset.h"
#include "WeaponModular/Public/HUD/UI/BUIUserWidget.h"
#include "ItemPartWidget.generated.h"

class UWeaponPartListWidget;
class UScrollBox;
class UButton;
class USlateBrushAsset;
enum class EWeaponGearPartSlot : uint8;
class USC_WeaponPartAttachmentPoint;

#pragma region Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnListButtonClick, UItemPartWidget*, ItemPartWidget);
#pragma endregion

/**
 * 
 */
UCLASS()
class WEAPONMODULAR_API UItemPartWidget : public UBUIUserWidget
{
	GENERATED_BODY()

public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	
	UPROPERTY(BlueprintAssignable)
	FOnListButtonClick OnListButtonClick;
	
	//====================================================================
	// FUNCTIONS
	//====================================================================
	UItemPartWidget();

	UFUNCTION(BlueprintCallable)
	void UpdateVisual();

	UFUNCTION()
	UWeaponPartListWidget* CreateWeaponPartListWidget();

	// Getters
	UDataTable* GetWidgetTable() {return WidgetTable;}
	UItemPartIconWidget* GetMainItemIconWidget() {return MainItemIconWidget; }
	UWeaponPartListWidget* GetPartListWidget() {return LinkedWeaponPartListWidget;}
	USC_WeaponPartAttachmentPoint* GetTargetMarkerLinked() {return TargetMarkerLinked;}
	FWeaponPartTypeProperties GetWidgetWeaponPartType() {return WidgetWeaponPartType;}

	//Setters
	void SetWidgetTable(UDataTable* NewDataTable) { WidgetTable = NewDataTable;	}
	void SetTargetMarkerLinked(USC_WeaponPartAttachmentPoint* NewWeaponPartAttachmentPoint) {TargetMarkerLinked =NewWeaponPartAttachmentPoint; }
	void SetWidgetWeaponPartType(FWeaponPartTypeProperties NewWeaponPartTypeProperties) {WidgetWeaponPartType = NewWeaponPartTypeProperties;}
	void SetBrushTargetPoint(FVector2D NewBrushTargetPoint) { BrushTargetPoint = NewBrushTargetPoint; }
	void SetBrushTexture(USlateBrushAsset* NewBrushTexture) { BrushTexture = NewBrushTexture; }

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
	TObjectPtr<USC_WeaponPartAttachmentPoint> TargetMarkerLinked;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> WidgetTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponPartTypeProperties WidgetWeaponPartType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USlateBrushAsset> BrushTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D BrushTargetPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D BrushTargetSize = FVector2D(16.0f, 16.0f);

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> IconMaterial;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeOnInitialized() override;

	
	UFUNCTION()
	void ListButtonClick();
	UFUNCTION()
	void PartClicked(UItemPartIconWidget* ItemPartIconWidget);
	
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
