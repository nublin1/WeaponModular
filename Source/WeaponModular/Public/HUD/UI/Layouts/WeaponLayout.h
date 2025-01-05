// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponModular/Public/HUD/UI/BUIUserWidget.h"
#include "WeaponLayout.generated.h"

class UCanvasPanel;
class UInventoryItemSlotWidget;
/**
 * 
 */
UCLASS()
class WEAPONMODULAR_API UWeaponLayout : public UBUIUserWidget
{
	GENERATED_BODY()

public:


protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCanvasPanel> MainCanvasPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UInventoryItemSlotWidget>> UInventoryItemSlotsWidgets;

	

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeConstruct() override;
};
