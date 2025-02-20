// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponPartListWidget.h"
#include "HUD/UI/BUIUserWidget.h"
#include "Settings/Settings.h"

#include "InventoryItemSlotWidget.generated.h"

class AInventorySceneRenderer;
class UScaleBox;
class USC_WeaponPartAttachmentPoint;
class ULineDrawerWidget;
class UImage;
class UUtilitiesRender;
class UCanvasPanel;
class UItemPartWidget;

#pragma region Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseMoveDelta, FVector2D, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseWheelDelta, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPartWidgetAdded, UItemPartWidget*, WidgetAdded);
#pragma endregion

#pragma region Enums

USTRUCT()
struct FItemsWidgetSlot
{
	GENERATED_BODY()

	UPROPERTY()
	FVector2D SlotPosition = FVector2D::ZeroVector;
	UPROPERTY()
	FVector2D ListSlotPosition = FVector2D::ZeroVector;
	UPROPERTY()
	UItemPartWidget* ItemPartWidgetLinked = nullptr;
};
#pragma endregion

UCLASS()
class WEAPONMODULAR_API UInventoryItemSlotWidget : public UBUIUserWidget
{
	GENERATED_BODY()
	
public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================	
	UPROPERTY(BlueprintAssignable)
	FOnMouseMoveDelta OnMouseMoveDelta;
	UPROPERTY(BlueprintAssignable)
	FOnMouseWheelDelta OnMouseWheelDelta;
	UPROPERTY(BlueprintAssignable)
	FOnItemPartWidgetAdded OnItemPartWidgetAdded;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	UFUNCTION(BlueprintCallable)
	void InitializeWidgetPositions();
	UFUNCTION(BlueprintCallable)
	void RecalculateLinesToDraw();
	UFUNCTION(BlueprintCallable)
	void ComparisonAndUpdateItemPartWidget(UItemPartWidget* Widget, USC_WeaponPartAttachmentPoint* AttachmentPoint);
	UFUNCTION(BlueprintCallable)
	void AddItemPartWidget (USC_WeaponPartAttachmentPoint* AttachmentPoint);
	UFUNCTION(BlueprintCallable)
	void CalculateItemSlotPositions(FVector2D size = FVector2D::ZeroVector);

	// Getters
	UFUNCTION(BlueprintCallable)
	TArray<FVector2D> GetItemsWidgetPositions();
	
	// Setters
	void SetRenderTargetMaterial(UTextureRenderTarget2D* RenderTarget);
	void SetUISettins(FUISettings NewUISettings) {UISettings = NewUISettings;}
	void SetRotationSettings(FRotationSettings NewSettings) {RotationSettings = NewSettings;}

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UCanvasPanel> MainCanvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCanvasPanel> ContentPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> RT_Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<ULineDrawerWidget> WBP_LineDrawer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	TArray<TObjectPtr<UItemPartWidget>> PartWidgets;
	

	// Data
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AInventorySceneRenderer> LinkedSceneRenderer;
	UPROPERTY()
	bool bIsDragging = false;
	UPROPERTY()
	FVector2D LastMousePosition;
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector2D> BoxesToDraw;

	// UI Settings
	UPROPERTY(BlueprintReadWrite)
	FUISettings UISettings;
	UPROPERTY()
	TArray<FItemsWidgetSlot> ItemWidgetsArray;

	// Rotation
	FVector2D RotationAngle = FVector2D::ZeroVector;	
	FRotationSettings RotationSettings;	

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeConstruct() override;

	
	UFUNCTION()
	FVector2D CalculateOvalPosition(int32 Index, const FVector2D& Center, float OvalWidth, float OvalHeight);
	UFUNCTION()
	FVector2D CalculateSquarePosition(int32 Index,const FVector2D& Center, float SquareWidth, float SquareHeight);
	UFUNCTION()
	FVector2D CalculateCirclePosition(int32 Index, const FVector2D& Center, float Radius, const FVector2D& ScreenSize) const;
	UFUNCTION()
	UItemPartWidget* CreateItemPartWidget();
	UFUNCTION()
	int32 FindIndexOfClosestAvaiableWidgetPosition(FVector2D ComparedPosition);
	UFUNCTION(BlueprintCallable)
	FVector2D CalculateCoordinates(USceneCaptureComponent2D* SceneCaptureComponent, FVector WorldPosition);
	UFUNCTION(BlueprintCallable)
	void CalculateLineToDraw(UItemPartWidget* ItemPartWidget);
	UWeaponPartListWidget* CreateAndPositionListWidget(UItemPartWidget* FromWidget);

	UFUNCTION()
	void ListButtonClick(UItemPartWidget* FromWidget);
	UFUNCTION()
	void UpdateWidgetsPositions();
	UFUNCTION()
	FVector2D Calculate3DRotationPosition(FVector2D Radius, FVector2D AngleOffset, float& OutDepth, FVector2D& BasePosition);
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


};
	
