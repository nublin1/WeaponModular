// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "WeaponPartData.generated.h"

UENUM(BlueprintType)
enum class EWeaponPartType : uint8
{
	None			UMETA(DisplayName = "None"),
	Essential		UMETA(DisplayName = "Essential"),     // Основные части (курок, ствол, приклад и т.д.)
	Attachment		UMETA(DisplayName = "Attachment")    // Навесные части (глушитель, прицел и т.д.)
};

UENUM(Blueprintable)
enum class EWeaponEssential : uint8
{
	None			UMETA(DisplayName = "None"),
	Barrel			UMETA(DisplayName = "Barrel"),
	Gunstock		UMETA(DisplayName = "Gunstock"),
	Magazine		UMETA(DisplayName = "Magazine"),
};

UENUM(Blueprintable)
enum class EWeaponAttachment : uint8
{
	None			UMETA(DisplayName = "None"),
	Barrel			UMETA(DisplayName = "BarrelAttachment"),
	Scope			UMETA(DisplayName = "Scope"),
};

USTRUCT(BlueprintType)
struct FWeaponPartTypeProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponPartType WeaponPartType = EWeaponPartType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "WeaponPartType == EWeaponPartType::Essential"))
	EWeaponEssential WeaponEssential = EWeaponEssential::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "WeaponPartType == EWeaponPartType::Attachment"))
	EWeaponAttachment WeaponAttachment = EWeaponAttachment::None;
};

USTRUCT(BlueprintType)
struct FWeaponPartAssets
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> StaticMesh = nullptr;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> DynamicMaterial = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Texture = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponPart
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponPartTypeProperties TypeProperties;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponPartAssets VisualProperties;
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


UCLASS()
class WEAPONMODULAR_API UWeaponPartDataUtilities : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static TArray<FWeaponPartData> GetSpecificWeaponParts(UDataTable* DataTable, EWeaponPartType PartType,
	                                                       uint8 SpecificTypeValue);

	UFUNCTION(BlueprintCallable)
	static bool AreWeaponPartPropertiesEqual(FWeaponPartTypeProperties WeaponPartTypeProperties, FWeaponPartTypeProperties RowName2);
};

inline TArray<FWeaponPartData> UWeaponPartDataUtilities::GetSpecificWeaponParts(UDataTable* DataTable,
	EWeaponPartType PartType, uint8 SpecificTypeValue)
{
	TArray<FWeaponPartData> MatchingParts;

	if (!DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
		return MatchingParts;
	}

	for (const auto& Row : DataTable->GetRowMap())
	{
		FWeaponPartData* WeaponPartData = reinterpret_cast<FWeaponPartData*>(Row.Value);
		if (!WeaponPartData)
			continue;
		
		const FWeaponPartTypeProperties& TypeProperties = WeaponPartData->BaseWeaponPartData.TypeProperties;
		if (TypeProperties.WeaponPartType == PartType)
		{
			bool bMatchesSpecificType = false;
			if (PartType == EWeaponPartType::Essential)
			{
				bMatchesSpecificType = (static_cast<uint8>(TypeProperties.WeaponEssential) == SpecificTypeValue);
			}
			else if (PartType == EWeaponPartType::Attachment)
			{
				bMatchesSpecificType = (static_cast<uint8>(TypeProperties.WeaponAttachment) == SpecificTypeValue);
			}
			
			if (bMatchesSpecificType)
			{
				MatchingParts.Add(*WeaponPartData);
			}
		}
	}

	return MatchingParts;
}

inline bool UWeaponPartDataUtilities::AreWeaponPartPropertiesEqual(FWeaponPartTypeProperties WeaponPartTypeProperties, FWeaponPartTypeProperties RowName2)
{
	if (WeaponPartTypeProperties.WeaponPartType != RowName2.WeaponPartType)
	{
		return false;
	}
	
	if (WeaponPartTypeProperties.WeaponPartType == EWeaponPartType::Essential)
	{
		if (WeaponPartTypeProperties.WeaponEssential != RowName2.WeaponEssential)
		{
			return false;
		}
	}
	else if (WeaponPartTypeProperties.WeaponPartType == EWeaponPartType::Attachment)
	{
		if (WeaponPartTypeProperties.WeaponAttachment != RowName2.WeaponAttachment)
		{
			return false;
		}
	}
	
	return true;
}