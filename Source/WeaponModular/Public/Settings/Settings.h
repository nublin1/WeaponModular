#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Settings.generated.h"

UENUM(Blueprintable)
enum class EWidgetsMethodLocation: uint8
{
	Oval			UMETA(DisplayName = "Oval"),
	Square			UMETA(DisplayName = "Square"),
	Circle			UMETA(DisplayName = "Circle"),
};

USTRUCT(Blueprintable)
struct FRotationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanRotate = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeed = 30.0f;
};

USTRUCT(Blueprintable)
struct FCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bReverseWeapon = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector InitialRelativeLocation = FVector(-50.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ECameraProjectionMode::Type> CameraProjectionMode = ECameraProjectionMode::Type::Perspective;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FOVAngle = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OrthoWidth = 140.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CameraOffsetSpeed = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinOffsetDistance = -30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxOffsetDistance = -1000.0f;
};

USTRUCT(Blueprintable)
struct FUISettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	int32 MaxItemWidgets = 12;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWidgetsMethodLocation WidgetsMethodLocation = EWidgetsMethodLocation::Oval;

	
};
