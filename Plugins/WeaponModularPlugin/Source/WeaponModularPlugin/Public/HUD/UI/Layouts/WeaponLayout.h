//  Nublin Studio 2025 All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "HUD/UI/BUIUserWidget.h"
#include "WeaponLayout.generated.h"

class UCanvasPanel;
class UInventoryItemSlotWidget;

/**
 * 
 */
UCLASS()
class WEAPONMODULARPLUGIN_API UWeaponLayout : public UBUIUserWidget
{
	GENERATED_BODY()

public:
	//====================================================================
	// FUNCTIONS
	//====================================================================
	UFUNCTION(BlueprintCallable, Category="Weapon Layout")
	void AddInventoryItemSlotsWidget(UInventoryItemSlotWidget* NewInventoryItemSlotsWidget);

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(BindWidget))
	TObjectPtr<UCanvasPanel> MainCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(BindWidget))
	TObjectPtr<UCanvasPanel> ContentPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<TObjectPtr<UInventoryItemSlotWidget>> UInventoryItemSlotsWidgets;

	// Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int CurrentChildIndexVisible = 0;

	// 
	TMap<UInventoryItemSlotWidget*, FTimerHandle> ActiveTimers;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category="Navigation")
	void OnNextButtonClicked();

	UFUNCTION(BlueprintCallable, Category="Navigation")
	void OnPreviousButtonClicked();
};
