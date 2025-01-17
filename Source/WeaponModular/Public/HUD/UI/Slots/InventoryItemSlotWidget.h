// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/UI/BUIUserWidget.h"
#include "Utilities/UtilitiesRender.h"
#include "InventoryItemSlotWidget.generated.h"

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

UENUM(Blueprintable)
enum class EWidgetsMethodLocation: uint8
{
	Oval			UMETA(DisplayName = "Oval"),
	Square			UMETA(DisplayName = "Square"),
	Circle			UMETA(DisplayName = "Circle"),
};

USTRUCT()
struct FItemsWidgetSlot
{
	GENERATED_BODY()

	UPROPERTY()
	FVector2D SlotPosition;
	UPROPERTY()
	UItemPartWidget* ItemPartWidgetLinked = nullptr;
};

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
	TArray<FVector2D> GetItemsWidgetPositions() {
		TArray<FVector2D> Result;
		for (auto Element : ItemsWidgetPositions)
		{
			Result.Add(Element.SlotPosition);
		}
		return Result;
	}
	
	// Setters
	void SetRenderTargetMaterial(UTextureRenderTarget2D* RenderTarget);

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

	// Classes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	TSubclassOf<UItemPartWidget> ItemPartWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> RenderTargetMaterial;

	// Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;
	UPROPERTY()
	bool bIsDragging = false;
	UPROPERTY()
	FVector2D LastMousePosition;
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector2D> BoxesToDraw;

	// ItemWidgets data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	int32 TotalItemWidgets = 12;
	UPROPERTY()
	TArray<FItemsWidgetSlot> ItemsWidgetPositions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	EWidgetsMethodLocation WidgetsMethodLocation = EWidgetsMethodLocation::Oval;

	// Rotation
	float RotationAngle = 0.0f;
	float RotationSpeed = 35.0f;
	

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void NativeConstruct() override;

	
	UFUNCTION()
	FVector2D CalculateOvalPosition(int32 Index, const FVector2D& Center, float OvalWidth, float OvalHeight);
	UFUNCTION()
	FVector2D CalculateSquarePosition(int32 Index,const FVector2D& Center, float SquareWidth, float SquareHeight);
	UFUNCTION()
	FVector2D CalculateCirclePosition(int32 Index, const FVector2D& Center, float Radius, const FVector2D& ScreenSize);
	UFUNCTION()
	UItemPartWidget* CreateItemPartWidget();
	UFUNCTION()
	int32 FindIndexOfClosestAvaiableWidgetPosition(FVector2D ComparedPosition);
	UFUNCTION(BlueprintCallable)
	FVector2D CalculateCoordinates(USceneCaptureComponent2D* SceneCaptureComponent, FVector WorldPosition);
	UFUNCTION(BlueprintCallable)
	void CalculateLineToDraw(UItemPartWidget* ItemPartWidget);

	UFUNCTION()
	void ListButtonClick(UItemPartWidget* FromWidget);
	UFUNCTION()
	void UpdateWidgetsPositions();
	UFUNCTION()
	FVector2D Calculate3DRotationPosition( float Radius, float AngleOffset, float& OutDepth, FVector2D& BasePosition);
	

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


};
	
