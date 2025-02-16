// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/UI/BUIUserWidget.h"
#include "Math/Color.h"
#include "LineDrawerWidget.generated.h"

USTRUCT(BlueprintType)
struct FLineDrawInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	FVector4 LineCoordinates; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	FLinearColor LineColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	float LineThickness = 1.0f;
};

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
	void ClearLinesToDraw();
	UFUNCTION(BlueprintCallable)
	bool AddLineToDraw(FString LineName, FVector2D StartPoint, FVector2D EndPoint, FLinearColor ColorLine = FLinearColor::Yellow, float Thickness = 1.0f);

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, FLineDrawInfo> LineDrawers;

	//====================================================================
	// FUNCTIONS
	//====================================================================
};
