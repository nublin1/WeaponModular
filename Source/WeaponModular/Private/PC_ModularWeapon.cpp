// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_ModularWeapon.h"

#include "Components/CanvasPanel.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Data/WeaponData.h"
#include "HUD/UI/Layouts/MainLayout.h"
#include "HUD/UI/Layouts/WeaponLayout.h"
#include "HUD/UI/Slots/InventoryItemSlotWidget.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "WeaponModular/Inv/AInventorySceneRenderer.h"
#include "World/Weapon.h"

APC_ModularWeapon::APC_ModularWeapon()
{
}

void APC_ModularWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (!WeaponTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
		return;
	}
	
}

void APC_ModularWeapon::ProcessWeaponTable()
{
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

void APC_ModularWeapon::SpawnInventorySceneRenderers(FWeaponData* WData)
{
	if (!InventorySceneRendererClass)
		return;
	
	FVector InstanceSpawnLocation = FVector(0.0f, 0.0f + IndexSpawn* 50.0f, 0.0f); 
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

	RendererInstance->CaptureComponent->AddRelativeLocation(FVector(-50.0f, 0.0f, 0.0f));
	//RendererInstance->CaptureComponent->ProjectionType = ECameraProjectionMode::Type::Orthographic;
	RendererInstance->CaptureComponent->OrthoWidth = 140.0f;
	RendererInstance->CaptureComponent->TextureTarget = RenderTarget;

	RendererInstance->ChildComponent->DestroyChildActor();
	RendererInstance->ChildComponent->SetChildActorClass(*WData->Weapon);

	RendererInstance->UpdateVisibleComponents();

	TObjectPtr<UInventoryItemSlotWidget> InventoryItemSlotWidget;

	if (WData->LinkedInventoryItemSlotWidget)
	{
		InventoryItemSlotWidget = CreateWidget<UInventoryItemSlotWidget>(GetWorld(), WData->LinkedInventoryItemSlotWidget);
	}
	else
	{
		if(!InventoryItemSlotWidgetClass)
			return;
		
		InventoryItemSlotWidget = CreateWidget<UInventoryItemSlotWidget>(GetWorld(), InventoryItemSlotWidgetClass);
	}

	if (!InventoryItemSlotWidget)
		return;
	
	MainHUDContainer->WeaponLayout->AddInventoryItemSlotsWidget(InventoryItemSlotWidget);
	InventoryItemSlotWidget->SetRenderTargetMaterial(RenderTarget);
}
