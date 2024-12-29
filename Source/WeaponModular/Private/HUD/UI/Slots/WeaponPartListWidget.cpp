// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/Slots/WeaponPartListWidget.h"

#include "Components/Image.h"
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
			UTexture* Texture;
			FVector2D TextureSize = FVector2D(256, 256);
			if (ListOfParts[i].BaseWeaponPartData.Material->GetTextureParameterValue(FName("BaseTexture"), Texture))
			{
				if (TObjectPtr<UTexture2D> Texture2D = Cast<UTexture2D>(Texture))
				{
					TextureSize.X = Texture2D->GetSizeX();
					TextureSize.Y = Texture2D->GetSizeY();

					//UE_LOG(LogTemp, Log, TEXT("Texture Size: Width=%d, Height=%d"), TextureWidth, TextureHeight);
				}
			}
			
			FSlateBrush Brush;
			Brush.SetResourceObject(ListOfParts[i].BaseWeaponPartData.Material);
			Brush.ImageSize = TextureSize;
			ItemPartIconWidget->GetContent_Image()->SetBrush(Brush);
			
			WeaponPartList_VerticalBox->AddChildToVerticalBox(ItemPartIconWidget);
		}
	}
}

void UWeaponPartListWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	this->SetVisibility(ESlateVisibility::Collapsed);
}
