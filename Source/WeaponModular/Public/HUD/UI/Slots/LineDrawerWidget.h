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

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY()
	TArray<FVector4> LineDrawers;

	//====================================================================
	// FUNCTIONS
	//====================================================================
};
