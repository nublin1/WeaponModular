//  Nublin Studio 2025 All Rights Reserved.

#include "HUD/UI/Slots/ItemPartWidget.h"

#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Helpers/SC_WeaponPartAttachmentPoint.h"
#include "HUD/UI/Slots/WeaponPartListWidget.h"

void UItemPartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ListButton->OnClicked.AddDynamic(this, &UItemPartWidget::ListButtonClick);
}

UItemPartWidget::UItemPartWidget()
{
	
}

void UItemPartWidget::UpdateVisual()
{
	if (!TargetMarkerLinked)
		return;

	TargetMarkerLinked->GetBrushTexture();

	auto RetrievedWeaponPartRow = TargetMarkerLinked->GetRetrievedWeaponPartData();
	if (!RetrievedWeaponPartRow)
	{
		
		MainItemIconWidget->GetContent_Image()->SetBrushFromTexture(nullptr);
		MainItemIconWidget->GetContent_Image()->SetOpacity(0.2f);
		MainItemIconWidget->GetContent_Text_Name()->SetText(FText::FromString("Empty"));
		return;
	}

	if (IconMaterial) 
	{
		if (UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(IconMaterial, this))
		{
			DynamicMaterial->SetTextureParameterValue(FName("BaseTexture"), RetrievedWeaponPartRow->BaseWeaponPartData.VisualProperties.Texture);
			MainItemIconWidget->GetContent_Image()->SetBrushFromMaterial(DynamicMaterial);
			MainItemIconWidget->GetContent_Image()->SetOpacity(1.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create dynamic material!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IconMaterial is null!"));
	}

	TargetMarkerLinked->GetBrushTexture();
	//MainItemIconWidget->GetContent_Image()->SetBrushFromMaterial(RetrievedWeaponPartRow->BaseWeaponPartData.VisualProperties.Material);

	FText Name = FText::FromString(RetrievedWeaponPartRow->Name.ToString());
	MainItemIconWidget->GetContent_Text_Name()->SetText(Name);
}

void UItemPartWidget::ListButtonClick()
{
	if (OnListButtonClick.IsBound())
		OnListButtonClick.Broadcast(this);
	
}

void UItemPartWidget::PartClicked(UItemPartIconWidget* ItemPartIconWidget)
{
	LinkedWeaponPartListWidget->SetVisibility(ESlateVisibility::Collapsed);

	auto Result = LinkedWeaponPartListWidget->GetItemPartIconToWeaponPartMap().Find(ItemPartIconWidget);

	if (!TargetMarkerLinked)
		return;
	TargetMarkerLinked->SetRetrievedWeaponPartData(Result);
	TargetMarkerLinked->UpdateStaticMeshComponent();

	UpdateVisual();
}

UWeaponPartListWidget* UItemPartWidget::CreateWeaponPartListWidget()
{
	if (!WeaponPartListWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponPartListWidgetClass is null!"));
		return nullptr;
	}

	if (UWeaponPartListWidget* WidgetInstance = CreateWidget<UWeaponPartListWidget>(
		GetWorld(), WeaponPartListWidgetClass))
	{
		auto WeaponParts = UWeaponPartDataUtilities::GetSpecificWeaponParts(
			WidgetTable,
			WidgetWeaponPartType.WeaponPartType
			);

		WidgetInstance->Initialized();
		WidgetInstance->ClearPartList();
		WidgetInstance->AddEmptyPartToList(); 
		if (WeaponParts.Num() > 0)
		{
			WidgetInstance->AddPartsToList(WeaponParts);
		}

		auto PartList = WidgetInstance->GetWeaponPartListBox();
		for (auto Part : PartList->GetAllChildren())
		{
			if (auto PartWidget = Cast<UItemPartIconWidget>(Part))
			{
				PartWidget->OnClickedContent_Button.AddDynamic(this, &UItemPartWidget::PartClicked);
			}
		}
		
		LinkedWeaponPartListWidget = WidgetInstance;
		return WidgetInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create WeaponPartListWidget!"));
		return nullptr;
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
