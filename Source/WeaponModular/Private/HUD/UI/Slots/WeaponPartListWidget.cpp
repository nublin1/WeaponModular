// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/Slots/WeaponPartListWidget.h"
#include "Data/WeaponPartData.h"
#include "Components/VerticalBox.h"
#include "HUD/UI/Slots/ItemPartIconWidget.h"



UWeaponPartListWidget::UWeaponPartListWidget()
{
	
}

void UWeaponPartListWidget::AddPartsToList(TArray<FWeaponPartData> ListOfParts)
{
	if (ListOfParts.Num() == 0)
		return;
	
	for (int32 i = 0; i < ListOfParts.Num(); i++)
	{
		if (TObjectPtr<UItemPartIconWidget> ItemPartIconWidget = CreateWidget<UItemPartIconWidget>(GetWorld(), ItemPartIconWidgetClass))
		{
			WeaponPartList_VerticalBox->AddChildToVerticalBox(ItemPartIconWidget);
		}
	}
}
