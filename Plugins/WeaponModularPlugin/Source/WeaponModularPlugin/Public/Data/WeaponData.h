//  Nublin Studio 2025 All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "WeaponData.generated.h"

class AWeapon;

USTRUCT(BlueprintType)
struct WEAPONMODULARPLUGIN_API FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<AWeapon> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon", meta = (ToolTip = "Optional: Leave empty if not needed"))
	TSubclassOf<UInventoryItemSlotWidget> LinkedInventoryItemSlotWidgetClass; 
};
