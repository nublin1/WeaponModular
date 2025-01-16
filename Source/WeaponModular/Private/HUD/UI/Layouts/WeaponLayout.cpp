// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModular/Public/HUD/UI/Layouts/WeaponLayout.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Components/ScaleBox.h"
#include "HUD/UI/Slots/InventoryItemSlotWidget.h"


void UWeaponLayout::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UInventoryItemSlotWidget*> ItemPartWidgets;
	TFunction<void(UWidget*, TArray<UInventoryItemSlotWidget*>&)> GatherChildWidgets = [&GatherChildWidgets
		](UWidget* ParentWidget, TArray<UInventoryItemSlotWidget*>& OutWidgets)
	{
		if (!ParentWidget) return;
		if (UInventoryItemSlotWidget* ItemPartWidget = Cast<UInventoryItemSlotWidget>(ParentWidget))
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

	if (MainCanvasPanel)
	{
		GatherChildWidgets(MainCanvasPanel.Get(), ItemPartWidgets);
	}

	if (ItemPartWidgets.Num() > 0)
		UInventoryItemSlotsWidgets = ItemPartWidgets;
}

void UWeaponLayout::AddInventoryItemSlotsWidget(UInventoryItemSlotWidget* NewInventoryItemSlotsWidget)
{
	UScaleBox* ScaleBox = NewObject<UScaleBox>(this);
	if (!ScaleBox)
	{
		return;
	}
	
	if (UCanvasPanelSlot* ScaleBoxSlot = Cast<UCanvasPanelSlot>(ContentPanel->AddChild(ScaleBox)))
	{
		ScaleBoxSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		ScaleBoxSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
		ScaleBoxSlot->SetAlignment(FVector2D(0.0f, 0.0f));
		ScaleBoxSlot->SetAutoSize(true);
	}
	
	auto Box = ScaleBox->AddChild(NewInventoryItemSlotsWidget);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this, NewInventoryItemSlotsWidget]()
		{
			NewInventoryItemSlotsWidget->CalculateItemSlotPositions();
		},
		0.1f,
		false
	);
	
	UInventoryItemSlotsWidgets.Add(NewInventoryItemSlotsWidget);
}
