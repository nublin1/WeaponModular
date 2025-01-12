// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/Slots/InventoryItemSlotWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Helpers/SC_WeaponPartAttachmentPoint.h"
#include "HUD/UI/Slots/ItemPartWidget.h"
#include "Materials/MaterialExpressionSceneColor.h"


void UInventoryItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UItemPartWidget*> ItemPartWidgets;
	TFunction<void(UWidget*, TArray<UItemPartWidget*>&)> GatherChildWidgets = [&GatherChildWidgets
		](UWidget* ParentWidget, TArray<UItemPartWidget*>& OutWidgets)
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

	if (ItemPartWidgets.Num() > 0)
		PartWidgets = ItemPartWidgets;
}

void UInventoryItemSlotWidget::AddItemPartWidget(USC_WeaponPartAttachmentPoint* AttachmentPoint)
{
	auto PosInScreen = CalculateCoordinates(CaptureComponent, AttachmentPoint->GetComponentLocation());
	auto Index = FindIndexOfClosestAvaiableWidgetPosition(PosInScreen);
	if (Index < 0)
		return;

	if (TObjectPtr<UItemPartWidget> NewItemWidget = CreateItemPartWidget())
	{
		//NewItemWidget->
		TObjectPtr<UCanvasPanelSlot> CanvasSlot = MainCanvas->AddChildToCanvas(NewItemWidget);
		if (CanvasSlot)
		{
			CanvasSlot->SetPosition(ItemsWidgetPositions[Index].SlotPosition);
			CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f));
			CanvasSlot->SetAutoSize(true);
			ItemsWidgetPositions[Index].bIsAvaiable = false;
			NewItemWidget->SetTargetMarkerLinked(AttachmentPoint);
			NewItemWidget->SetWidgetTable(static_cast<UDataTable*>(AttachmentPoint->GetWeaponPartRow().DataTable));
			NewItemWidget->SetWidgetWeaponPartType(AttachmentPoint->WeaponPointType);
			NewItemWidget->UpdateVisual();
			
			PartWidgets.Add(NewItemWidget);
		}
	}
}

void UInventoryItemSlotWidget::CalculateItemSlotPositions()
{
	const FVector2D WidgetSize = GetCachedGeometry().GetLocalSize();
	const FVector2D Center(WidgetSize.X / 2.0f, WidgetSize.Y / 2.0f);

	for (int32 i = 0; i < TotalItemWidgets; ++i)
	{
		FVector2D Position;
		switch (WidgetsMethodLocation)
		{
		case EWidgetsMethodLocation::Oval:
			{
				float OvalWidth = WidgetSize.X * 0.8f;
				float OvalHeight = WidgetSize.Y * 0.7f;
				Position = CalculateOvalPosition(i, Center, OvalWidth, OvalHeight);
				break;
			}

		case EWidgetsMethodLocation::Square:
			{
				float SquareWidth = WidgetSize.X * 0.8f;
				float SquareHeight = WidgetSize.Y * 0.8f;
				Position = CalculateSquarePosition(i, Center, SquareWidth, SquareHeight);
				break;
			}
		}

		FItemsWidgetSlot NewItemsWidgetSlot;
		NewItemsWidgetSlot.SlotPosition = Position;
		ItemsWidgetPositions.Add(NewItemsWidgetSlot);
	}
}

FVector2D UInventoryItemSlotWidget::CalculateOvalPosition(int32 Index, const FVector2D& Center,
                                                          float OvalWidth, float OvalHeight)
{
	float Angle = 2.0f * PI * Index / TotalItemWidgets;
	float X = Center.X + (OvalWidth / 2.0f) * FMath::Cos(Angle);
	float Y = Center.Y + (OvalHeight / 2.0f) * FMath::Sin(Angle);
	float PerspectiveFactor = 1.0f + 0.2f * FMath::Abs(FMath::Sin(Angle)); // Увеличиваем разрыв в центре

	//return FVector2D(X, Y);
	return FVector2D(X, Y * PerspectiveFactor);
}

FVector2D UInventoryItemSlotWidget::CalculateSquarePosition(int32 Index, const FVector2D& Center,
                                                            float SquareWidth, float SquareHeight)
{
	float Perimeter = 2.0f * (SquareWidth + SquareHeight);
	float StepLength = Perimeter / TotalItemWidgets;

	float CurrentLength = StepLength * Index;
	FVector2D Position;

	if (CurrentLength <= SquareWidth) // Up
	{
		Position = FVector2D(Center.X - SquareWidth / 2.0f + CurrentLength, Center.Y - SquareHeight / 2.0f);
	}
	else if (CurrentLength <= SquareWidth + SquareHeight) // Right
	{
		CurrentLength -= SquareWidth;
		Position = FVector2D(Center.X + SquareWidth / 2.0f, Center.Y - SquareHeight / 2.0f + CurrentLength);
	}
	else if (CurrentLength <= 2.0f * SquareWidth + SquareHeight) // Down
	{
		CurrentLength -= (SquareWidth + SquareHeight);
		Position = FVector2D(Center.X + SquareWidth / 2.0f - CurrentLength, Center.Y + SquareHeight / 2.0f);
	}
	else // Left
	{
		CurrentLength -= (2.0f * SquareWidth + SquareHeight);
		Position = FVector2D(Center.X - SquareWidth / 2.0f, Center.Y + SquareHeight / 2.0f - CurrentLength);
	}

	return Position;
}

UItemPartWidget* UInventoryItemSlotWidget::CreateItemPartWidget()
{
	if (!ItemPartWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemPartWidgetClass is not set!"));
		return nullptr;
	}

	return CreateWidget<UItemPartWidget>(GetWorld(), ItemPartWidgetClass);
}

int32 UInventoryItemSlotWidget::FindIndexOfClosestAvaiableWidgetPosition(FVector2D ComparedPosition)
{
	int32 Index = -1;
	float ClosestDistance = FLT_MAX;
	
	for (int32 i = 0; i < ItemsWidgetPositions.Num(); ++i)
	{
		float Distance = FVector2D::Distance(ComparedPosition, ItemsWidgetPositions[i].SlotPosition);
		if (Distance < ClosestDistance &&  ItemsWidgetPositions[i].bIsAvaiable == true)
		{
			ClosestDistance = Distance;
			Index = i;
		}
	}
	
	return Index;
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
