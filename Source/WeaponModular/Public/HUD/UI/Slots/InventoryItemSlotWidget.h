// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/UI/BUIUserWidget.h"
#include "Utilities/UtilitiesRender.h"
#include "InventoryItemSlotWidget.generated.h"

class ULineDrawerWidget;
class UImage;
class UUtilitiesRender;
class UCanvasPanel;
class UItemPartWidget;

#pragma region Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveDelta, FVector2D, Delta);
#pragma endregion

UCLASS()
class WEAPONMODULAR_API UInventoryItemSlotWidget   : public UBUIUserWidget
{
	GENERATED_BODY()
	
public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(BlueprintAssignable)
	FOnMoveDelta OnMoveDelta;

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCanvasPanel> MainCanvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> RT_Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<ULineDrawerWidget> WBP_LineDrawer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	TArray<TObjectPtr<UItemPartWidget>> PartWidgets;

	// Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;
	UPROPERTY()
	bool bIsDragging = false;
	UPROPERTY()
	FVector2D LastMousePosition;
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector2D> BoxesToDraw;
	

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	FVector2D CalculateCoordinates(USceneCaptureComponent2D* SceneCaptureComponent, FVector WorldPosition);
	
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
	
