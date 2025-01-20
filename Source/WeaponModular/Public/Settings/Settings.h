#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Settings.generated.h"

USTRUCT(Blueprintable)
struct FRotationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanRotate = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationSpeed = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ZoomSpeed = 20.0f;
};

USTRUCT(Blueprintable)
struct FCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector InitialRelativeLocation = FVector(-50.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ECameraProjectionMode::Type> CameraProjectionMode = ECameraProjectionMode::Type::Perspective;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FOVAngle = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OrthoWidth = 140.0f;
};
