#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h" 
#include "WeaponPartData.generated.h"

UENUM(Blueprintable)
enum class EWeaponGearPartType : uint8
{
	None			UMETA(DisplayName = "None"),
	Barrel			UMETA(DisplayName = "Barrel"),
	Magazine		UMETA(DisplayName = "Magazine"),
	Butt			UMETA(DisplayName = "Butt"),
	Scope			UMETA(DisplayName = "Scope"),
};

USTRUCT(BlueprintType)
struct FBaseWeaponPart
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponGearPartType WeaponGearPartType;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> UStaticMesh = nullptr;

	UPROPERTY(EditAnywhere)
	bool bUseRenderTarget = false; 

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseRenderTarget"))
	TObjectPtr<UTextureRenderTarget2D> TextureRenderTarget = nullptr;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bUseRenderTarget"))
	TObjectPtr<UTexture> Texture = nullptr;
};

USTRUCT()
struct WEAPONMODULAR_API FWeaponPartData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBaseWeaponPart BaseWeaponPartData;
};