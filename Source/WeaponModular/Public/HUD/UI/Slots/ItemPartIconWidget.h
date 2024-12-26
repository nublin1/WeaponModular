// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/UI/BUIUserWidget.h"
#include "ItemPartIconWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class WEAPONMODULAR_API UItemPartIconWidget : public UBUIUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	UImage* GetContent_Image() {return Content_Image;}

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> Content_Image;
};
