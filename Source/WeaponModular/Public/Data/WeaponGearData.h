#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h" 
#include "WeaponGearData.generated.h"

UENUM(Blueprintable)
enum class EWeaponGearPartSlot : uint8
{
	None			UMETA(DisplayName = "None"),
	Barrel			UMETA(DisplayName = "Barrel"),
	Scope			UMETA(DisplayName = "Scope"),
};

USTRUCT(BlueprintType)
struct FWeaponGearPart
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponGearPartSlot WeaponGearPartType = EWeaponGearPartSlot::None;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> UStaticMesh = nullptr;

	UPROPERTY(EditAnywhere)
	bool bUseRenderTarget = false; 

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseRenderTarget"))
	TObjectPtr<UTextureRenderTarget2D> TextureRenderTarget = nullptr;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseRenderTarget"))
	TObjectPtr<UTexture> Texture = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> Material = nullptr;
};

USTRUCT()
struct WEAPONMODULAR_API FWeaponGearData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponGearPart BaseWeaponPartData;
};