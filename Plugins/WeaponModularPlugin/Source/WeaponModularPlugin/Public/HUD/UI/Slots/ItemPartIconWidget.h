// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/UI/BUIUserWidget.h"
#include "ItemPartIconWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

#pragma region Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedContent_Button, UItemPartIconWidget*, ItemPartIconWidget);
#pragma endregion

UCLASS()
class WEAPONMODULARPLUGIN_API UItemPartIconWidget : public UBUIUserWidget
{
	GENERATED_BODY()

public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================

	/** Delegate for button click event */
	UPROPERTY(BlueprintAssignable, Category = "Item Part Icon|Delegates")
	FOnClickedContent_Button OnClickedContent_Button;

	//====================================================================
	// FUNCTIONS
	//====================================================================

	UItemPartIconWidget();

	virtual void NativeOnInitialized() override;

	/** Returns the button used for content interaction */
	UFUNCTION(BlueprintCallable, Category = "Item Part Icon|Accessors")
	UButton* GetContent_Button() { return Content_Button; }

	/** Returns the image used for content display */
	UFUNCTION(BlueprintCallable, Category = "Item Part Icon|Accessors")
	UImage* GetContent_Image() { return Content_Image; }

	/** Returns the text block used for displaying the item name */
	UFUNCTION(BlueprintCallable, Category = "Item Part Icon|Accessors")
	UTextBlock* GetContent_Text_Name() { return Content_Text_Name; }

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================

	/** Button used for content interaction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Part Icon|Widgets", meta = (BindWidget))
	TObjectPtr<UButton> Content_Button;

	/** Image used for content display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Part Icon|Widgets", meta = (BindWidget))
	TObjectPtr<UImage> Content_Image;

	/** Text block used for displaying the item name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Part Icon|Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> Content_Text_Name;

	//====================================================================
	// FUNCTIONS
	//====================================================================

	/** Handles the button click event */
	UFUNCTION(Category = "Item Part Icon|Event Handlers")
	void Content_ButtonClick();
};
