// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMainLayout> MainHUDContainer;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> WeaponTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInventorySceneRenderer> InventorySceneRendererClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventoryItemSlotWidget> InventoryItemSlotWidgetClass;

	// Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AInventorySceneRenderer>> InventorySceneRendererList;

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
