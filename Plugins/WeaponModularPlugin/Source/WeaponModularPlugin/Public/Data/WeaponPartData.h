#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "WeaponPartData.generated.h"

UENUM(Blueprintable)
enum class EWeaponPartType : uint8
{
	None				UMETA(DisplayName = "None"),
	BarrelAttachment	UMETA(DisplayName = "Barrel Attachment"),
	Scope				UMETA(DisplayName = "Scope"),
	Barrel				UMETA(DisplayName = "Barrel"),
	Gunstock			UMETA(DisplayName = "Gunstock"),
	Magazine			UMETA(DisplayName = "Magazine"),
};

USTRUCT(BlueprintType)
struct FWeaponPartTypeProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Part")
	EWeaponPartType WeaponPartType = EWeaponPartType::None;
};

USTRUCT(BlueprintType)
struct FWeaponPartAssets
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Part|Assets")
	TObjectPtr<UStaticMesh> StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Part|Assets")
	TObjectPtr<UTexture2D> Texture = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponPart
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Part")
	FWeaponPartTypeProperties TypeProperties;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Part")
	FWeaponPartAssets VisualProperties;
};

USTRUCT(BlueprintType)
struct WEAPONMODULARPLUGIN_API FWeaponPartData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Part Data")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Part Data")
	FWeaponPart BaseWeaponPartData;
};

UCLASS()
class WEAPONMODULARPLUGIN_API UWeaponPartDataUtilities : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Weapon Part Utilities")
	static TArray<FWeaponPartData> GetSpecificWeaponParts(UDataTable* DataTable, EWeaponPartType PartType);

	UFUNCTION(BlueprintCallable, Category="Weapon Part Utilities")
	static bool AreWeaponPartPropertiesEqual(FWeaponPartTypeProperties WeaponPartTypeProperties, FWeaponPartTypeProperties RowName2);
};

inline TArray<FWeaponPartData> UWeaponPartDataUtilities::GetSpecificWeaponParts(UDataTable* DataTable, EWeaponPartType PartType)
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
			MatchingParts.Add(*WeaponPartData);
		}
	}

	return MatchingParts;
}

inline bool UWeaponPartDataUtilities::AreWeaponPartPropertiesEqual(FWeaponPartTypeProperties WeaponPartTypeProperties, FWeaponPartTypeProperties RowName2)
{
	return WeaponPartTypeProperties.WeaponPartType == RowName2.WeaponPartType;
}