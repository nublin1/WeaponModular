// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UtilitiesRender.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONMODULARPLUGIN_API  UUtilitiesRender : public UObject
{
	GENERATED_BODY()

public:
	//====================================================================
	// FUNCTIONS
	//====================================================================
	
	/** 
	* De-projects screen space coordinates to world space
	* @param Target - The SceneCaptureComponent2D for the camera
	* @param ScreenPos - Screen position to be de-projected
	* @param OutWorldOrigin - Output world origin
	* @param OutWorldDirection - Output world direction
	*/
	UFUNCTION(BlueprintCallable, Category = "Utilities|Rendering")
	static void CaptureComponent2D_DeProject(class USceneCaptureComponent2D* Target, const FVector2D& ScreenPos, FVector& OutWorldOrigin, FVector& OutWorldDirection);

	/** 
	* Converts a world position to texture coordinates
	* @param SceneCaptureComponent - The SceneCaptureComponent2D for the camera
	* @param WorldPosition - The world position to be converted
	* @return The corresponding texture coordinates
	*/
	UFUNCTION(BlueprintCallable, Category = "Utilities|Rendering")
	static FVector2D WorldToTextureCoordinates(USceneCaptureComponent2D* SceneCaptureComponent, const FVector& WorldPosition);
};
