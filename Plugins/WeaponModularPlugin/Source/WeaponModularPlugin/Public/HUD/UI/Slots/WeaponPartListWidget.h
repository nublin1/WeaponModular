// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponPartData.h"
#include "HUD/UI/BUIUserWidget.h"
#include "WeaponPartListWidget.generated.h"


class AWeaponModularManager;
class UScrollBox;
struct FWeaponPartData;
class UVerticalBox;
class UItemPartIconWidget;
/**
 * 
 */
UCLASS()
class WEAPONMODULARPLUGIN_API  UWeaponPartListWidget : public UBUIUserWidget
{
	GENERATED_BODY()

public:
	//====================================================================
	// FUNCTIONS
	//====================================================================
	
	/** Constructor for UWeaponPartListWidget */
	UWeaponPartListWidget();

	/** Initializes the widget */
	UFUNCTION(BlueprintCallable, Category = "Weapon Part List|Initialization")
	void Initialized();
	
	/** Clears the part list */
	UFUNCTION(BlueprintCallable, Category = "Weapon Part List|Management")
	void ClearPartList() const;

	/** Adds an empty part to the list */
	UFUNCTION(BlueprintCallable, Category = "Weapon Part List|Management")
	void AddEmptyPartToList();

	/** Adds a list of weapon parts to the list */
	UFUNCTION(BlueprintCallable, Category = "Weapon Part List|Management")
	void AddPartsToList(TArray<FWeaponPartData> ListOfParts);

	//====================================================================
	// GETTERS
	//====================================================================

	/** Returns the scroll box for the weapon part list */
	UFUNCTION(BlueprintCallable, Category = "Weapon Part List|Accessors")
	UScrollBox* GetWeaponPartList_ScrollBox() {return WeaponPartList_ScrollBox;}

	/** Returns the vertical box for the weapon part list */
	UFUNCTION(BlueprintCallable, Category = "Weapon Part List|Accessors")
	UVerticalBox* GetWeaponPartListBox() {return WeaponPartList_VerticalBox;}

	/** Returns a map of item part icons to weapon part data */
	TMap<TObjectPtr<UItemPartIconWidget>, FWeaponPartData> GetItemPartIconToWeaponPartMap() {return ItemPartIconToWeaponPartMap;}

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Part List|Widgets", meta = (BindWidget))
	TObjectPtr<UScrollBox> WeaponPartList_ScrollBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Part List|Widgets", meta = (BindWidget))
	TObjectPtr<UVerticalBox> WeaponPartList_VerticalBox;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon Part List|Data")
	TMap<TObjectPtr<UItemPartIconWidget>, FWeaponPartData> ItemPartIconToWeaponPartMap;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Part List|References")
	TObjectPtr<AWeaponModularManager> WeaponModularManager;

	//====================================================================
	// FUNCTIONS
	//====================================================================

	/** Handles mouse leave event */
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
