// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponPartListWidget.h"
#include "Engine/TextureRenderTarget2D.h"
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
class WEAPONMODULARPLUGIN_API UInventoryItemSlotWidget : public UBUIUserWidget
{
	GENERATED_BODY()
	
public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnMouseMoveDelta OnMouseMoveDelta;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnMouseWheelDelta OnMouseWheelDelta;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnItemPartWidgetAdded OnItemPartWidgetAdded;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void InitializeWidgetPositions();

	UFUNCTION(BlueprintCallable, Category = "Update")
	void RecalculateLinesToDraw();

	UFUNCTION(BlueprintCallable, Category = "Update")
	void ComparisonAndUpdateItemPartWidget(UItemPartWidget* Widget, USC_WeaponPartAttachmentPoint* AttachmentPoint);

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void AddItemPartWidget(USC_WeaponPartAttachmentPoint* AttachmentPoint);

	UFUNCTION(BlueprintCallable, Category = "Positioning")
	void CalculateItemSlotPositions(FVector2D size = FVector2D::ZeroVector);

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Positioning")
	TArray<FVector2D> GetItemsWidgetPositions();
	
	// Setters
	void SetRenderTargetMaterial(UTextureRenderTarget2D* RenderTarget);
	void SetUISettings(FUISettings NewUISettings) { UISettings = NewUISettings; }
	void SetRotationSettings(FRotationSettings NewSettings) { RotationSettings = NewSettings; }

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetOptional), Category = "Widgets")
	TObjectPtr<UCanvasPanel> MainCanvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget), Category = "Widgets")
	TObjectPtr<UCanvasPanel> ContentPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget), Category = "Widgets")
	TObjectPtr<UImage> RT_Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetOptional), Category = "Widgets")
	TObjectPtr<ULineDrawerWidget> WBP_LineDrawer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	TArray<TObjectPtr<UItemPartWidget>> PartWidgets;
	
	// Data
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TObjectPtr<AInventorySceneRenderer> LinkedSceneRenderer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Data")
	bool bIsDragging = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Transient, Category = "Data")
	FVector2D LastMousePosition;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TArray<FVector2D> BoxesToDraw;

	// UI Settings
	UPROPERTY(BlueprintReadWrite, Category = "UI Settings")
	FUISettings UISettings;

	UPROPERTY(Transient)
	TArray<FItemsWidgetSlot> ItemWidgetsArray;

	// Rotation
	UPROPERTY(Transient)
	FVector2D RotationAngle = FVector2D::ZeroVector;

	UPROPERTY(Transient)
	FRotationSettings RotationSettings;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeConstruct() override;

	UFUNCTION(Category = "Positioning")
	FVector2D CalculateOvalPosition(int32 Index, const FVector2D& Center, float OvalWidth, float OvalHeight);

	UFUNCTION(Category = "Positioning")
	FVector2D CalculateSquarePosition(int32 Index, const FVector2D& Center, float SquareWidth, float SquareHeight);

	UFUNCTION(Category = "Positioning")
	FVector2D CalculateCirclePosition(int32 Index, const FVector2D& Center, float Radius, const FVector2D& ScreenSize) const;

	UFUNCTION(Category = "Widgets")
	UItemPartWidget* CreateItemPartWidget();

	UFUNCTION(Category = "Widgets")
	int32 FindIndexOfClosestAvailableWidgetPosition(FVector2D ComparedPosition);

	UFUNCTION(BlueprintCallable, Category = "Positioning")
	FVector2D CalculateCoordinates(USceneCaptureComponent2D* SceneCaptureComponent, FVector WorldPosition);

	UFUNCTION(BlueprintCallable, Category = "Drawing")
	void CalculateLineToDraw(UItemPartWidget* ItemPartWidget);

	UFUNCTION(Category = "Widgets")
	UWeaponPartListWidget* CreateAndPositionListWidget(UItemPartWidget* FromWidget);

	UFUNCTION(Category = "Widgets")
	void ListButtonClick(UItemPartWidget* FromWidget);

	UFUNCTION(Category = "Widgets")
	void UpdateWidgetsPositions();

	UFUNCTION(Category = "Positioning")
	FVector2D Calculate3DRotationPosition(FVector2D Radius, FVector2D AngleOffset, float& OutDepth, FVector2D& BasePosition);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

};
