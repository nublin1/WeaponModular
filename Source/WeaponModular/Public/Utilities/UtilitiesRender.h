// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UtilitiesRender.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONMODULAR_API UUtilitiesRender : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void CaptureComponent2D_DeProject(class USceneCaptureComponent2D* Target, const FVector2D& ScreenPos, FVector& OutWorldOrigin, FVector& OutWorldDirection);

	UFUNCTION(BlueprintCallable)
	static FVector2D WorldToTextureCoordinates(USceneCaptureComponent2D* SceneCaptureComponent, const FVector& WorldPosition);
};
