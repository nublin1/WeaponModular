// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/Slots/WeaponPartListWidget.h"

#include "PC_ModularWeapon.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/WeaponPartData.h"
#include "Components/VerticalBox.h"
#include "HUD/UI/Slots/ItemPartIconWidget.h"


UWeaponPartListWidget::UWeaponPartListWidget()
{
	
}

void UWeaponPartListWidget::ClearPartList() const
{
	WeaponPartList_VerticalBox->ClearChildren();
}

void UWeaponPartListWidget::AddEmptyPartToList()
{
	if (TObjectPtr<UItemPartIconWidget> ItemPartIconWidget = CreateWidget<UItemPartIconWidget>(GetWorld(), ItemPartIconWidgetClass))
	{
		WeaponPartList_VerticalBox->AddChildToVerticalBox(ItemPartIconWidget);
	}
}

void UWeaponPartListWidget::AddPartsToList(TArray<FWeaponPartData> ListOfParts)
{
	if (ListOfParts.Num() == 0)
		return;
	
	for (int32 i = 0; i < ListOfParts.Num(); i++)
	{
		if (TObjectPtr<UItemPartIconWidget> ItemPartIconWidget = CreateWidget<UItemPartIconWidget>(GetWorld(), ItemPartIconWidgetClass))
		{
			UTexture* Texture = ListOfParts[i].BaseWeaponPartData.VisualProperties.Texture;
			FVector2D TextureSize = FVector2D(256, 256);
			if (Texture)
			{
				if (TObjectPtr<UTexture2D> Texture2D = Cast<UTexture2D>(Texture))
				{
					TextureSize.X = Texture2D->GetSizeX();
					TextureSize.Y = Texture2D->GetSizeY();
					//UE_LOG(LogTemp, Log, TEXT("Texture Size: Width=%d, Height=%d"), TextureWidth, TextureHeight);
				}
			}

			auto PC =Cast<APC_ModularWeapon>(GetOwningLocalPlayer()->GetPlayerController(GetWorld()));
			
			
			FSlateBrush Brush;
			if (PC->GetUISettings().IconMaterial)
			{
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(PC->GetUISettings().IconMaterial, this);
				DynamicMaterial->SetTextureParameterValue(FName("BaseTexture"), ListOfParts[i].BaseWeaponPartData.VisualProperties.Texture);
				Brush.SetResourceObject(DynamicMaterial);
			}
			
			Brush.ImageSize = TextureSize;
			ItemPartIconWidget->GetContent_Image()->SetBrush(Brush);
			ItemPartIconWidget->GetContent_Image()->SetOpacity(1.0f);
			FText Name = FText::FromString(ListOfParts[i].Name.ToString());
			ItemPartIconWidget->GetContent_Text_Name()->SetText(Name);
			
			WeaponPartList_VerticalBox->AddChildToVerticalBox(ItemPartIconWidget);
			ItemPartIconToWeaponPartMap.Add(ItemPartIconWidget, ListOfParts[i]);
		}
	}
}

void UWeaponPartListWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	//this->SetVisibility(ESlateVisibility::Collapsed);
}
