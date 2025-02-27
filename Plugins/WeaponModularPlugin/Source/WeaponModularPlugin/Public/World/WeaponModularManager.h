//  Nublin Studio 2025 All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Settings/Settings.h"
#include "WeaponModularManager.generated.h"

class UInventoryItemSlotWidget;
class UMainLayout;
struct FWeaponData;
class AInventorySceneRenderer;

UCLASS()
class WEAPONMODULARPLUGIN_API AWeaponModularManager : public AActor
{
	GENERATED_BODY()
	
public:	
	//====================================================================
	// FUNCTIONS
	//====================================================================

	AWeaponModularManager();

	// Getters
	UFUNCTION(Category = "Settings")
	FUISettings GetUISettings() { return UISettings; }

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================

	// Main UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UMainLayout> MainHUDContainer;

	// Weapon and Inventory Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	TObjectPtr<UDataTable> WeaponTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AInventorySceneRenderer> InventorySceneRendererClass;

	// Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<TObjectPtr<AInventorySceneRenderer>> InventorySceneRendererList;
	
	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FRotationSettings RotationSettings;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FCameraSettings CameraSettings;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FUISettings UISettings;
	
	// Spawn Index
	int IndexSpawn = 0;

	//====================================================================
	// FUNCTIONS
	//====================================================================

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon Processing")
	void ProcessWeaponTable();
	
	void SpawnInventorySceneRenderers(FWeaponData* WData);
};
