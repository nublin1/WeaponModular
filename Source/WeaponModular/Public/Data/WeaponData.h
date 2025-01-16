#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "WeaponData.generated.h"


class AWeapon;

USTRUCT()
struct WEAPONMODULAR_API FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWeapon> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInventoryItemSlotWidget> LinkedInventoryItemSlotWidget; 
};
