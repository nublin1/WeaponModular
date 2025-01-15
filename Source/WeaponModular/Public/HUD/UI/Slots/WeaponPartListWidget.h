// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponPartData.h"
#include "HUD/UI/BUIUserWidget.h"
#include "WeaponPartListWidget.generated.h"


class UScrollBox;
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

	UFUNCTION()
	void ClearPartList() const;
	UFUNCTION()
	void AddEmptyPartToList();
	UFUNCTION()
	void AddPartsToList(TArray<FWeaponPartData> ListOfParts);

	//Getters
	UScrollBox* GetWeaponPartList_ScrollBox() {return WeaponPartList_ScrollBox;}
	UVerticalBox* GetWeaponPartListBox() {return WeaponPartList_VerticalBox;}
	TMap<TObjectPtr<UItemPartIconWidget>, FWeaponPartData> GetItemPartIconToWeaponPartMap() {return ItemPartIconToWeaponPartMap;}
	
protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UScrollBox> WeaponPartList_ScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UVerticalBox> WeaponPartList_VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemPartIconWidget> ItemPartIconWidgetClass;

	// Data
	UPROPERTY(BlueprintReadOnly)
	TMap<TObjectPtr<UItemPartIconWidget>, FWeaponPartData> ItemPartIconToWeaponPartMap;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	
	
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
