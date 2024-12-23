// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModular/Public/HUD/UI/Slots/ItemPartWidget.h"

#include "GameplayTagsManager.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "HUD/UI/Slots/WeaponPartListWidget.h"

void UItemPartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWeaponPartsByType();

	ListButton->OnClicked.AddDynamic(this, &UItemPartWidget::UItemPartWidget::ListButtonClick);
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

void UItemPartWidget::ListButtonClick()
{
	if (!WeaponPartListWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponPartListWidgetClass is null!"));
		return;
	}
	if (UWeaponPartListWidget* WidgetInstance = CreateWidget<UWeaponPartListWidget>(GetWorld(), WeaponPartListWidgetClass))
	{
		auto Position = Cast<UCanvasPanelSlot>(this->Slot)->GetPosition();
		//auto Position = this->GetCachedGeometry().GetAbsolutePosition();
		auto Size =  this->GetCachedGeometry().GetLocalSize();

		FVector2D ResultPosition = Position;
		
		WidgetInstance->AddToViewport(50);
		WidgetInstance->SetPositionInViewport(ResultPosition);
		WidgetInstance-> SetDesiredSizeInViewport(FVector2D(100, 100 +50));
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateLambda([WidgetInstance]()
			{
				FVector2D ResultPosition2 = WidgetInstance->GetCachedGeometry().GetAbsolutePosition();
				UE_LOG(LogTemp, Warning, TEXT("ResultPosition X: %f, ResultPosition Y: %f"), ResultPosition2.X, ResultPosition2.Y);
				
				FVector2D Size2 = WidgetInstance->GetCachedGeometry().GetAbsoluteSize();
				UE_LOG(LogTemp, Warning, TEXT("Size2 X: %f, Size2 Y: %f"), Size2.X, Size2.Y);
			}),
			0.2f, // Задержка перед проверкой (100 мс)
			false
		);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create WeaponPartListWidget!"));
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
