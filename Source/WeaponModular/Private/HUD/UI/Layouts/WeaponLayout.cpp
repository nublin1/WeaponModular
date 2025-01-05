// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModular/Public/HUD/UI/Layouts/WeaponLayout.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/PanelWidget.h"
#include "HUD/UI/Slots/InventoryItemSlotWidget.h"

void UWeaponLayout::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UInventoryItemSlotWidget*> ItemPartWidgets; 
	TFunction<void(UWidget*, TArray<UInventoryItemSlotWidget*>&)> GatherChildWidgets = [&GatherChildWidgets](UWidget* ParentWidget, TArray<UInventoryItemSlotWidget*>& OutWidgets)
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

	if (ItemPartWidgets.Num()>0)
		UInventoryItemSlotsWidgets = ItemPartWidgets;
}
