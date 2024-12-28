// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModular/Public/HUD/UI/Slots/ItemPartWidget.h"

#include "GameplayTagsManager.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "Engine/UserInterfaceSettings.h"
#include "HUD/UI/Slots/WeaponPartListWidget.h"
#include "Kismet/GameplayStatics.h"

void UItemPartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWeaponPartsByType();

	ListButton->OnClicked.AddDynamic(this, &UItemPartWidget::UItemPartWidget::ListButtonClick);
}

TArray<FWeaponPartData> UItemPartWidget::GetWeaponPartsByType()
{
	TArray<FWeaponPartData> FilteredParts;
	if (!WidgetTable) // Проверяем, что DataTable существует
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetTable is null!"));
		return FilteredParts;
	}

	TArray<FName> RowNames = WidgetTable->GetRowNames();
	if (RowNames.Num() == 0)
		return FilteredParts;
	
	for (const FName& RowName : RowNames)
	{
		FWeaponPartData* Row = WidgetTable->FindRow<FWeaponPartData>(RowName, TEXT(""));

		if (Row && Row->BaseWeaponPartData.WeaponGearPartType == WidgetType)
		{
			FilteredParts.Add(*Row); 
		}
	}

	return FilteredParts;
}

void UItemPartWidget::ListButtonClick()
{
	if (!LinkedWeaponPartListWidget)
	{
		CreateWeaponPartListWidget();
	}

	
	
}

void UItemPartWidget::CreateWeaponPartListWidget()
{
	if (!WeaponPartListWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponPartListWidgetClass is null!"));
		return;
	}
	
	if (UWeaponPartListWidget* WidgetInstance = CreateWidget<UWeaponPartListWidget>(GetWorld(), WeaponPartListWidgetClass))
	{
		auto CountParts = GetWeaponPartsByType();
		if (CountParts.Num() > 0)
		{
			WidgetInstance->AddPartsToList(CountParts);
		}
		
		FVector2D RenderCurrentPosition = this->GetCachedGeometry().GetAbsolutePosition();		
		FVector2D CurrentSize = this->GetCachedGeometry().GetLocalSize();
				
		FVector2D PixelPosition;
		FVector2D ViewportPosition;
		USlateBlueprintLibrary::AbsoluteToViewport(
			GetWorld(),
			RenderCurrentPosition,
			PixelPosition, ViewportPosition);
		//UE_LOG(LogTemp, Warning, TEXT("ViewportPosition X: %f, ViewportPosition Y: %f"), ViewportPosition.X, ViewportPosition.Y);

		FVector2D FinalViewportPosition = FVector2D(ViewportPosition.X, ViewportPosition.Y + CurrentSize.Y);
		
		LinkedWeaponPartListWidget = WidgetInstance;
		WidgetInstance->AddToViewport(1);
		WidgetInstance->SetPositionInViewport(FinalViewportPosition, false);
		WidgetInstance->SetDesiredSizeInViewport(FVector2D(CurrentSize.X+20, CurrentSize.Y + 50));

		/*// Проверка после небольшой задержки
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateLambda([WidgetInstance]()
			{
				FVector2D ResultPosition2 = WidgetInstance->GetCachedGeometry().GetAbsolutePositionAtCoordinates(FVector2D(0, 0));
				UE_LOG(LogTemp, Warning, TEXT("ResultPosition X: %f, ResultPosition Y: %f"), ResultPosition2.X, ResultPosition2.Y);

				FVector2D Size2 = WidgetInstance->GetCachedGeometry().GetAbsoluteSize();
				UE_LOG(LogTemp, Warning, TEXT("Size2 X: %f, Size2 Y: %f"), Size2.X, Size2.Y);
			}),
			0.2f, // Задержка перед проверкой
			false
		);*/
		
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
