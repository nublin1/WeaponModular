#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Camera/CameraTypes.h"
#include "Settings.generated.h"

class UItemPartIconWidget;
class UWeaponPartListWidget;
class UItemPartWidget;
class UInventoryItemSlotWidget;

UENUM(Blueprintable)
enum class EWidgetsMethodLocation : uint8
{
	Oval	UMETA(DisplayName = "Oval"),
	Square	UMETA(DisplayName = "Square"),
	Circle	UMETA(DisplayName = "Circle"),
};

USTRUCT(Blueprintable)
struct FRotationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation")
	bool bCanRotate = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation")
	float RotationSpeed = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation|Limits")
	float XAxisMinClamp = -60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation|Limits")
	float XAxisMaxClamp = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation|Limits")
	float YAxisMinClamp = -45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation|Limits")
	float YAxisMaxClamp = 45.0f;
};

USTRUCT(Blueprintable)
struct FCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	bool bReverseWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	FVector InitialRelativeLocation = FVector(-50.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	TEnumAsByte<ECameraProjectionMode::Type> CameraProjectionMode = ECameraProjectionMode::Type::Perspective;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	float FOVAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	float OrthoWidth = 140.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Centering")
	bool bCenterWeapon = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Centering")
	float CameraOffsetSpeed = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Centering")
	float MinOffsetDistance = -30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera|Centering")
	float MaxOffsetDistance = -1000.0f;
};

USTRUCT(Blueprintable)
struct FUISettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	int32 MaxItemWidgets = 12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	EWidgetsMethodLocation WidgetsMethodLocation = EWidgetsMethodLocation::Oval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Widgets")
	TSubclassOf<UInventoryItemSlotWidget> InventoryItemSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Materials")
	TObjectPtr<UMaterialInterface> BaseRenderTargetMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Widgets")
	TSubclassOf<UItemPartWidget> ItemPartWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Widgets")
	TSubclassOf<UWeaponPartListWidget> WeaponPartListWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Widgets")
	TSubclassOf<UItemPartIconWidget> ItemPartIconWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Materials")
	TObjectPtr<UMaterialInterface> IconMaterial;
};
