// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/Slots/WeaponPartListWidget.h"

#include "PC_ModularWeapon.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/WeaponPartData.h"
#include "Components/VerticalBox.h"
#include "HUD/UI/Slots/ItemPartIconWidget.h"
#include "Kismet/GameplayStatics.h"
#include "World/WeaponModularManager.h"


UWeaponPartListWidget::UWeaponPartListWidget()
{
	
}

void UWeaponPartListWidget::Initialized()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeaponModularManager::StaticClass(), FoundActors);
	
			
	if (FoundActors.Num() > 0)
	{
		WeaponModularManager =  Cast<AWeaponModularManager>(FoundActors[0]);
	}
}

void UWeaponPartListWidget::ClearPartList() const
{
	if (!WeaponPartList_VerticalBox)
		return;
	
	WeaponPartList_VerticalBox->ClearChildren();
}

void UWeaponPartListWidget::AddEmptyPartToList()
{
	if (!WeaponModularManager)
		return;
	
	if (TObjectPtr<UItemPartIconWidget> ItemPartIconWidget = CreateWidget<UItemPartIconWidget>(GetWorld(),  WeaponModularManager->GetUISettings().ItemPartIconWidgetClass))
	{
		WeaponPartList_VerticalBox->AddChildToVerticalBox(ItemPartIconWidget);
	}
}

void UWeaponPartListWidget::AddPartsToList(TArray<FWeaponPartData> ListOfParts)
{
	if (ListOfParts.Num() == 0)
		return;
	
	if (!WeaponModularManager)
		return;

	if (WeaponModularManager->GetUISettings().ItemPartIconWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemPartIconWidgetClass is null!"));
		return;
	}
	
	for (int32 i = 0; i < ListOfParts.Num(); i++)
	{
		if (TObjectPtr<UItemPartIconWidget> ItemPartIconWidget = CreateWidget<UItemPartIconWidget>(GetWorld(), WeaponModularManager->GetUISettings().ItemPartIconWidgetClass))
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
			
			
			FSlateBrush Brush;
			if (WeaponModularManager)
			{
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(WeaponModularManager->GetUISettings().IconMaterial, this);
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
