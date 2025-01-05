// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "WeaponPartData.generated.h"

UENUM(Blueprintable)
enum class EWeaponPartSlot : uint8
{
	None			UMETA(DisplayName = "None"),
	Barrel			UMETA(DisplayName = "Barrel"),
	Gunstock		UMETA(DisplayName = "Gunstock"),
};

USTRUCT(BlueprintType)
struct FWeaponPart
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponPartSlot WeaponPartType = EWeaponPartSlot::None;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> UStaticMesh = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> Material = nullptr;
};

USTRUCT()
struct WEAPONMODULAR_API FWeaponPartData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponPart BaseWeaponPartData;
};
