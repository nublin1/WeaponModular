// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Settings/Settings.h"
#include "PC_ModularWeapon.generated.h"

class UInventoryItemSlotWidget;
class UMainLayout;
struct FWeaponData;
class AInventorySceneRenderer;
/**
 * 
 */
UCLASS()
class WEAPONMODULAR_API APC_ModularWeapon : public APlayerController
{
	GENERATED_BODY()

public:
	//====================================================================
	// FUNCTIONS
	//====================================================================
	APC_ModularWeapon();

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UMainLayout> MainHUDContainer;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> WeaponTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInventorySceneRenderer> InventorySceneRendererClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventoryItemSlotWidget> InventoryItemSlotWidgetClass;

	// Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AInventorySceneRenderer>> InventorySceneRendererList;
	

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FRotationSettings RotationSettings;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FCameraSettings CameraSettings;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FUISettings  UISettings;
	
	//
	int IndexSpawn = 0;

	//====================================================================
	// FUNCTIONS
	//====================================================================

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ProcessWeaponTable();
	
	void SpawnInventorySceneRenderers(FWeaponData* WData);
	
};
