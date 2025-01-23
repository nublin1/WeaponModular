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
	if (!Box)
		return;
	
	FTimerHandle& TimerHandle = ActiveTimers.FindOrAdd(NewInventoryItemSlotsWidget);
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this, NewInventoryItemSlotsWidget, ScaleBox]()
		{
			if (!NewInventoryItemSlotsWidget->GetCachedGeometry().Size.IsZero())
			{				
				NewInventoryItemSlotsWidget->CalculateItemSlotPositions(NewInventoryItemSlotsWidget->GetCachedGeometry().Size);
				ScaleBox->SetVisibility(ESlateVisibility::Collapsed);
				
				FTimerHandle* FoundHandle = ActiveTimers.Find(NewInventoryItemSlotsWidget);
				if (FoundHandle)
				{
					GetWorld()->GetTimerManager().ClearTimer(*FoundHandle);
					ActiveTimers.Remove(NewInventoryItemSlotsWidget);
				}
			}
		},
		0.1f, 
		true  
	);

	UInventoryItemSlotsWidgets.Add(NewInventoryItemSlotsWidget);
}

void UWeaponLayout::OnNextButtonClicked()
{
	if (!ContentPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("ContentPanel is null!"));
		return;
	}
	if (UWidget* CurrentWidget = ContentPanel->GetChildAt(CurrentChildIndexVisible))
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	const int32 ChildrenCount = ContentPanel->GetChildrenCount();
	if (ChildrenCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ContentPanel has no children!"));
		return;
	}

	CurrentChildIndexVisible = (CurrentChildIndexVisible + 1) % ChildrenCount;
	if (UWidget* NextWidget = ContentPanel->GetChildAt(CurrentChildIndexVisible))
	{
		NextWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWeaponLayout::OnPreviousButtonClicked()
{
	if (!ContentPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("ContentPanel is null!"));
		return;
	}
	
	if (UWidget* CurrentWidget = ContentPanel->GetChildAt(CurrentChildIndexVisible))
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	const int32 ChildrenCount = ContentPanel->GetChildrenCount();
	if (ChildrenCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ContentPanel has no children!"));
		return;
	}

	CurrentChildIndexVisible = (CurrentChildIndexVisible - 1 + ChildrenCount) % ChildrenCount;
	if (UWidget* PreviousWidget = ContentPanel->GetChildAt(CurrentChildIndexVisible))
	{
		PreviousWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
