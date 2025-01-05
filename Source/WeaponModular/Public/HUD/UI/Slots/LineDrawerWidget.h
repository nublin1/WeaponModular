// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/UI/BUIUserWidget.h"
#include "LineDrawerWidget.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONMODULAR_API ULineDrawerWidget : public UBUIUserWidget
{
	GENERATED_BODY()

public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	

	//====================================================================
	// FUNCTIONS
	//====================================================================
	ULineDrawerWidget();
	
	UFUNCTION(BlueprintCallable)
	bool AddLineToDraw(FString LineName, FVector2D StartPoint, FVector2D EndPoint);

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, FVector4> LineDrawers;

	//====================================================================
	// FUNCTIONS
	//====================================================================
};
