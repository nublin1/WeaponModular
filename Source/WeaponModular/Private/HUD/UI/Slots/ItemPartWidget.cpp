// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModular/Public/HUD/UI/Slots/ItemPartWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Helpers/SC_WeaponPartAttachmentPoint.h"
#include "HUD/UI/Slots/WeaponPartListWidget.h"

void UItemPartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//ListButton->OnClicked.AddDynamic(this, &UItemPartWidget::ListButtonClick);
}

void UItemPartWidget::UpdateVisual()
{
	if (!TargetMarkerLinked)
		return;

	auto RetrievedWeaponPartRow = TargetMarkerLinked->GetRetrievedWeaponPartData();
	if (!RetrievedWeaponPartRow)
	{
		MainItemIconWidget->GetContent_Image()->SetBrushFromTexture(nullptr);
		MainItemIconWidget->GetContent_Image()->SetOpacity(0.2f);
		MainItemIconWidget->GetContent_Text_Name()->SetText(FText::FromString("Empty"));
		return;
	}

	MainItemIconWidget->GetContent_Image()->SetBrushFromMaterial(RetrievedWeaponPartRow->BaseWeaponPartData.VisualProperties.Material);
	MainItemIconWidget->GetContent_Image()->SetOpacity(1.0f);
	FText Name = FText::FromString(RetrievedWeaponPartRow->Name.ToString());
	MainItemIconWidget->GetContent_Text_Name()->SetText(Name);
}

void UItemPartWidget::ListButtonClick()
{
	
	
	/*if (!LinkedWeaponPartListWidget)
	{
		CreateWeaponPartListWidget();
	}
	else
	{
		if (LinkedWeaponPartListWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			LinkedWeaponPartListWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			LinkedWeaponPartListWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}*/
}

void UItemPartWidget::PartClicked(UItemPartIconWidget* ItemPartIconWidget)
{
	LinkedWeaponPartListWidget->SetVisibility(ESlateVisibility::Collapsed);

	auto Result = LinkedWeaponPartListWidget->GetItemPartIconToWeaponPartMap().Find(ItemPartIconWidget);

	TargetMarkerLinked->SetRetrievedWeaponPartData(Result);
	TargetMarkerLinked->UpdateStaticMeshComponent();

	UpdateVisual();
}

void UItemPartWidget::CreateWeaponPartListWidget()
{
	if (!WeaponPartListWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponPartListWidgetClass is null!"));
		return;
	}

	if (UWeaponPartListWidget* WidgetInstance = CreateWidget<UWeaponPartListWidget>(
		GetWorld(), WeaponPartListWidgetClass))
	{
		auto CountParts = UWeaponPartDataUtilities::GetSpecificWeaponParts(
			WidgetTable,
			WidgetWeaponPartType.WeaponPartType,
			WidgetWeaponPartType.WeaponPartType == EWeaponPartType::Essential?
				static_cast<int>(WidgetWeaponPartType.WeaponEssential) : static_cast<int>(WidgetWeaponPartType.WeaponAttachment)
			);

		WidgetInstance->ClearPartList();
		WidgetInstance->AddEmptyPartToList(); 
		if (CountParts.Num() > 0)
		{
			WidgetInstance->AddPartsToList(CountParts);
		}

		auto PartList = WidgetInstance->GetWeaponPartListBox();
		for (auto Part : PartList->GetAllChildren())
		{
			if (auto PartWidget = Cast<UItemPartIconWidget>(Part))
			{
				PartWidget->OnClickedContent_Button.AddDynamic(this, &UItemPartWidget::PartClicked);
			}
		}

		FVector2D RenderCurrentPosition = this->GetCachedGeometry().GetAbsolutePosition();
		FVector2D CurrentSize = this->GetCachedGeometry().GetAbsoluteSize();
		FVector2D LocalSize = this->GetDesiredSize();
		float Scale = this->GetCachedGeometry().GetAccumulatedLayoutTransform().GetScale();
		FVector2D ModPosition = FVector2D(RenderCurrentPosition.X, RenderCurrentPosition.Y + CurrentSize.Y);

		FVector2D PixelPosition;
		FVector2D ViewportPosition;
		USlateBlueprintLibrary::AbsoluteToViewport(
			GetWorld(),
			ModPosition,
			PixelPosition, ViewportPosition);
		UE_LOG(LogTemp, Warning, TEXT("CurrentSize X: %f, CurrentSize Y: %f"), CurrentSize.X, CurrentSize.Y);
		UE_LOG(LogTemp, Warning, TEXT("CurrentSize X: %s"),* GetParent()->GetParent()->GetName());

		FVector2D FinalViewportPosition = ViewportPosition;

		WidgetInstance->AddToViewport(1);
		WidgetInstance->SetPositionInViewport(FinalViewportPosition, false);
		WidgetInstance->SetDesiredSizeInViewport(FVector2D(
			(CurrentSize.X),
			(CountParts.Num() > 0? CurrentSize.Y + 50 : CurrentSize.Y ) 
		));
		

		LinkedWeaponPartListWidget = WidgetInstance;

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
