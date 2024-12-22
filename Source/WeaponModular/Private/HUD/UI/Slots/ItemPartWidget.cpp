// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModular/Public/HUD/UI/Slots/ItemPartWidget.h"

#include "GameplayTagsManager.h"
#include "Components/Button.h"

void UItemPartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWeaponPartsByType();
}

void UItemPartWidget::GetWeaponPartsByType()
{
	TArray<FWeaponPartData> FilteredParts;
	if (!WidgetTable) // Проверяем, что DataTable существует
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetTable is null!"));
		return;
	}

	TArray<FName> RowNames = WidgetTable->GetRowNames();
	if (RowNames.Num() == 0)
		return;
	
	for (const FName& RowName : RowNames)
	{
		FWeaponPartData* Row = WidgetTable->FindRow<FWeaponPartData>(RowName, TEXT(""));

		if (Row && Row->BaseWeaponPartData.WeaponGearPartType == WidgetType)
		{
			FilteredParts.Add(*Row); 
		}
	}
}

void UItemPartWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (ListButton)
	{
		ListButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemPartWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (ListButton)
	{
		ListButton->SetVisibility(ESlateVisibility::Hidden);
	}
}
