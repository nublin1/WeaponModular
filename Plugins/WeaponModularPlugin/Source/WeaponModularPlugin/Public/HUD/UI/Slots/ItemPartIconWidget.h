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

/**
 * 
 */
UCLASS()
class WEAPONMODULARPLUGIN_API UItemPartIconWidget : public UBUIUserWidget
{
	GENERATED_BODY()

public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	FOnClickedContent_Button OnClickedContent_Button;
	
	//====================================================================
	// FUNCTIONS
	//====================================================================
	UItemPartIconWidget();

	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	UButton* GetContent_Button() {return Content_Button;}
	UFUNCTION()
	UImage* GetContent_Image() {return Content_Image;}
	UFUNCTION()
	UTextBlock* GetContent_Text_Name() {return Content_Text_Name;}

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UButton> Content_Button;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> Content_Image;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> Content_Text_Name;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	UFUNCTION()
	void Content_ButtonClick();

	
};
