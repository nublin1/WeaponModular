// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemPartIconWidget.h"
#include "Data/WeaponPartData.h"
#include "WeaponPartListWidget.h"
#include "Helpers/SC_WeaponPartAttachmentPoint.h"
#include "HUD/UI/BUIUserWidget.h"
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
class WEAPONMODULARPLUGIN_API  UItemPartWidget : public UBUIUserWidget
{
	GENERATED_BODY()

public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnListButtonClick OnListButtonClick;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	UItemPartWidget();

	UFUNCTION(BlueprintCallable, Category = "Update")
	void UpdateVisual();

	UFUNCTION(Category = "Widgets")
	UWeaponPartListWidget* CreateWeaponPartListWidget();

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Widgets")
	UDataTable* GetWidgetTable() { return WidgetTable; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	UItemPartIconWidget* GetMainItemIconWidget() { return MainItemIconWidget; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	UWeaponPartListWidget* GetPartListWidget() { return LinkedWeaponPartListWidget; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	USC_WeaponPartAttachmentPoint* GetTargetMarkerLinked() { return TargetMarkerLinked; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	FVector2D GetBrushTargetPoint() const { return BrushTargetPoint; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	USlateBrushAsset* GetBrushTexture() { return TargetMarkerLinked->GetBrushTexture(); }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	FWeaponPartTypeProperties GetWidgetWeaponPartType() { return WidgetWeaponPartType; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	FVector2D& GetBrushTargetSize() { return BrushTargetSize; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	TSubclassOf<UWeaponPartListWidget>& GetWeaponPartListWidgetClass() { return WeaponPartListWidgetClass; }

	// Setters
	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void SetWidgetTable(UDataTable* NewDataTable) { WidgetTable = NewDataTable; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void SetTargetMarkerLinked(USC_WeaponPartAttachmentPoint* NewWeaponPartAttachmentPoint) { TargetMarkerLinked = NewWeaponPartAttachmentPoint; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void SetWidgetWeaponPartType(FWeaponPartTypeProperties NewWeaponPartTypeProperties) { WidgetWeaponPartType = NewWeaponPartTypeProperties; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void SetBrushTargetPoint(FVector2D NewBrushTargetPoint) { BrushTargetPoint = NewBrushTargetPoint; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void SetIconMaterial(UMaterialInterface* NewMaterial) { IconMaterial = NewMaterial; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void SetBrushTargetSize(const FVector2D& _BrushTargetSize) { this->BrushTargetSize = _BrushTargetSize; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void SetWeaponPartListWidgetClass(const TSubclassOf<UWeaponPartListWidget>& _WeaponPartListWidgetClass) { this->WeaponPartListWidgetClass = _WeaponPartListWidgetClass; }

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Widgets")
	TObjectPtr<UItemPartIconWidget> MainItemIconWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Widgets")
	TObjectPtr<UButton> ListButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UWeaponPartListWidget> WeaponPartListWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TObjectPtr<UWeaponPartListWidget> LinkedWeaponPartListWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TObjectPtr<USC_WeaponPartAttachmentPoint> TargetMarkerLinked;

	// Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataTable> WidgetTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FWeaponPartTypeProperties WidgetWeaponPartType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FVector2D BrushTargetPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FVector2D BrushTargetSize = FVector2D(16.0f, 16.0f);

	// Materials
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TObjectPtr<UMaterialInterface> IconMaterial;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeOnInitialized() override;

	UFUNCTION(Category = "Actions")
	void ListButtonClick();

	UFUNCTION(Category = "Actions")
	void PartClicked(UItemPartIconWidget* ItemPartIconWidget);

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
