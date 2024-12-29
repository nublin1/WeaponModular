// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/UI/BUIUserWidget.h"
#include "WeaponPartListWidget.generated.h"

struct FWeaponPartData;
class UVerticalBox;
class UItemPartIconWidget;
/**
 * 
 */
UCLASS()
class WEAPONMODULAR_API UWeaponPartListWidget : public UBUIUserWidget
{
	GENERATED_BODY()

public:
	//====================================================================
	// FUNCTIONS
	//====================================================================
	UWeaponPartListWidget();

	void AddPartsToList(TArray<FWeaponPartData> ListOfParts);
	
protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UVerticalBox> WeaponPartList_VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemPartIconWidget> ItemPartIconWidgetClass;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
