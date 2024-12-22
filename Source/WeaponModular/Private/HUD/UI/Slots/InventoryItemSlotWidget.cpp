// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/Slots/InventoryItemSlotWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "HUD/UI/Slots/ItemPartWidget.h"
#include "Materials/MaterialExpressionSceneColor.h"


void UInventoryItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UItemPartWidget*> ItemPartWidgets; 
	TFunction<void(UWidget*, TArray<UItemPartWidget*>&)> GatherChildWidgets = [&GatherChildWidgets](UWidget* ParentWidget, TArray<UItemPartWidget*>& OutWidgets)
	{
		if (!ParentWidget) return;
		if (UItemPartWidget* ItemPartWidget = Cast<UItemPartWidget>(ParentWidget))
		{
			OutWidgets.Add(ItemPartWidget);
		}
		
		if (UPanelWidget* Panel = Cast<UPanelWidget>(ParentWidget))
		{
			for (int32 i = 0; i < Panel->GetChildrenCount(); ++i)
			{
				GatherChildWidgets(Panel->GetChildAt(i), OutWidgets);
			}
		}
	};

	if (MainCanvas)
	{
		GatherChildWidgets(MainCanvas.Get(), ItemPartWidgets);
	}

	if (ItemPartWidgets.Num()>0)
		PartWidgets = ItemPartWidgets;
}

FVector2D UInventoryItemSlotWidget::CalculateCoordinates(USceneCaptureComponent2D* SceneCaptureComponent,
	FVector WorldPosition)
{
	auto WPositions = WorldPosition;
	
	auto Result = UUtilitiesRender::WorldToTextureCoordinates(SceneCaptureComponent, WPositions);

	return FVector2D(Result.X, Result.Y);
}

FReply UInventoryItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = true; 
		LastMousePosition = InMouseEvent.GetScreenSpacePosition();
		
		return FReply::Handled().CaptureMouse(TakeWidget());
	}
	return Reply;
}

FReply UInventoryItemSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (bIsDragging && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = false;
		return FReply::Handled().ReleaseMouseCapture();
	}
	return Reply;
}

FReply UInventoryItemSlotWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (bIsDragging)
	{
		FVector2D CurrentMousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D Delta = CurrentMousePosition - LastMousePosition;
		LastMousePosition = CurrentMousePosition;
		//UE_LOG(LogTemp, Warning, TEXT("Delta X: %f, Delta Y: %f"), Delta.X, Delta.Y);

		if (OnMoveDelta.IsBound())
			OnMoveDelta.Broadcast(Delta);
	}

	if (!HasMouseCapture())
	{
		bIsDragging = false;
		return FReply::Unhandled();
	}

	return Reply;
}
