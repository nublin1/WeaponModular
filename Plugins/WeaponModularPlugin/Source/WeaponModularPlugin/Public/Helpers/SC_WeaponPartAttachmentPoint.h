//  Nublin Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Data/WeaponPartData.h"
#include "Styling/SlateBrush.h"
#include "SC_WeaponPartAttachmentPoint.generated.h"

#pragma region Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaticMeshUpdated);
#pragma endregion

class UImage;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONMODULARPLUGIN_API USC_WeaponPartAttachmentPoint : public USceneComponent
{
	GENERATED_BODY()

public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(BlueprintAssignable, Category="Weapon Part")
	FOnStaticMeshUpdated OnStaticMeshUpdated;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Part")
	FWeaponPartTypeProperties WeaponPointType;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	USC_WeaponPartAttachmentPoint();

	UFUNCTION(BlueprintCallable, Category="Weapon Part")
	virtual void Initialize();

	UFUNCTION(BlueprintCallable, Category="Weapon Part")
	virtual void UpdateStaticMeshComponent();

	// Getters
	bool GetIsNeedDrawLine() const { return bIsNeedDrawLine; }
	FLinearColor GetLineColor() const { return LineColor; }
	float GetLineThickness() const { return LineThickness; }
	USlateBrushAsset* GetBrushTexture() { return BrushTexture; }
	UDataTable* GetUsableTable() { return UsableTable; }
	FDataTableRowHandle GetInitialWeaponPartRow() { return InitialWeaponPartRow; }
	FWeaponPartData* GetRetrievedWeaponPartData() { return RetrievedWeaponPartData; }

	// Setters
	void SetRetrievedWeaponPartData(FWeaponPartData* NewWeaponPartData);

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
	bool bIsNeedDrawLine = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals", meta = (EditCondition = "bIsNeedDrawLine == true"))
	FLinearColor LineColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals", meta = (EditCondition = "bIsNeedDrawLine == true"))
	float LineThickness = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
	TObjectPtr<USlateBrushAsset> BrushTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Part")
	TObjectPtr<UDataTable> UsableTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Part")
	FDataTableRowHandle InitialWeaponPartRow;

	FWeaponPartData* RetrievedWeaponPartData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Part")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
