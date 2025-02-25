// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "World/WeaponModularManager.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Data/WeaponData.h"
#include "HUD/UI/Layouts/MainLayout.h"
#include "HUD/UI/Layouts/WeaponLayout.h"
#include "HUD/UI/Slots/InventoryItemSlotWidget.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "AInventorySceneRenderer.h"
#include "World/Weapon.h"

AWeaponModularManager::AWeaponModularManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AWeaponModularManager::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponModularManager::ProcessWeaponTable()
{
	if (!WeaponTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
		return;
	}
	
	const TMap<FName, uint8*>& RowMap = WeaponTable->GetRowMap();

	for (const auto& Row : RowMap)
	{
		FName RowName = Row.Key; 
		auto RowData = Row.Value;

		if (FWeaponData* WData = WeaponTable->FindRow<FWeaponData>(RowName, RowName.ToString()))
		{
			SpawnInventorySceneRenderers(WData);
		}
	}
}

void AWeaponModularManager::SpawnInventorySceneRenderers(FWeaponData* WData)
{
	if (!InventorySceneRendererClass)
	{
		UE_LOG(LogTemp, Log, TEXT("InventorySceneRendererClass is null"));
		return;
	}
	
	FVector InstanceSpawnLocation = FVector(0.0f, 0.0f + IndexSpawn* 50.0f, -1000.0f); 
	FRotator InstanceSpawnRotation = FRotator::ZeroRotator;
	
	AInventorySceneRenderer* RendererInstance = GetWorld()->SpawnActor<AInventorySceneRenderer>(
		InventorySceneRendererClass,
		InstanceSpawnLocation,
		InstanceSpawnRotation
	);

	if (!RendererInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to spawn InventorySceneRenderer"));
		return;
	}

	InventorySceneRendererList.Add(RendererInstance);
	IndexSpawn++;

	FVector2D ViewportSize = FVector2D( 1, 1 );
	ViewportSize.X = GSystemResolution.ResX;
	ViewportSize.Y = GSystemResolution.ResY;

	UTextureRenderTarget2D* RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, ViewportSize.X, ViewportSize.Y, RTF_RGBA16f);
	if (!RenderTarget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create RenderTarget."));
		return;
	}
	
	RendererInstance->SetRotationSettings(RotationSettings);
	RendererInstance->SetCameraSettings(CameraSettings);
	RendererInstance->Initialize();
	RendererInstance->CaptureComponent->TextureTarget = RenderTarget;
	
	//RendererInstance->ChildComponent->DestroyChildActor();
	RendererInstance->ChildComponent->SetChildActorClass(*WData->Weapon);

	RendererInstance->UpdateVisibleComponents();

	TObjectPtr<UInventoryItemSlotWidget> InventoryItemSlotWidget;
	if (WData->LinkedInventoryItemSlotWidgetClass)
	{
		InventoryItemSlotWidget = CreateWidget<UInventoryItemSlotWidget>(GetWorld(), WData->LinkedInventoryItemSlotWidgetClass);
	}
	else
	{
		if(!UISettings.InventoryItemSlotWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("InventoryItemSlotWidgetClass is null."));
			return;
		}
		
		InventoryItemSlotWidget = CreateWidget<UInventoryItemSlotWidget>(GetWorld(), UISettings.InventoryItemSlotWidgetClass);
	}

	if (!InventoryItemSlotWidget)
		return;
	
	MainHUDContainer->WeaponLayout->AddInventoryItemSlotsWidget(InventoryItemSlotWidget);
	InventoryItemSlotWidget->SetUISettings(UISettings);
	InventoryItemSlotWidget->SetRenderTargetMaterial(RenderTarget);
	InventoryItemSlotWidget->SetRotationSettings(RotationSettings);
	
}
