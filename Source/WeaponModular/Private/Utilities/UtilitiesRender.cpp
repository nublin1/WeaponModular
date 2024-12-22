// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/UtilitiesRender.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMathLibrary.h"

void UUtilitiesRender::CaptureComponent2D_DeProject(class USceneCaptureComponent2D* Target, const FVector2D& ScreenPos,
                                                    FVector& OutWorldOrigin, FVector& OutWorldDirection)
{
	const FTransform& Transform = Target->GetComponentToWorld();
	FMatrix ViewMatrix = Transform.ToInverseMatrixWithScale();
	FVector ViewLocation = Transform.GetTranslation();

	// swap axis st. x=z,y=x,z=y (unreal coord space) so that z is up
	ViewMatrix = ViewMatrix * FMatrix(
		FPlane(0,    0,    1,    0),
		FPlane(1,    0,    0,    0),
		FPlane(0,    1,    0,    0),
		FPlane(0,    0,    0,    1));

	const float FOV = Target->FOVAngle * (float)PI / 360.0f;
	
	FIntPoint CaptureSize(Target->TextureTarget->GetSurfaceWidth(),
		Target->TextureTarget->GetSurfaceHeight());

	float XAxisMultiplier;
	float YAxisMultiplier;

	if (CaptureSize.X > CaptureSize.Y)
	{
		// if the viewport is wider than it is tall
		XAxisMultiplier = 1.0f;
		YAxisMultiplier = CaptureSize.X / static_cast<float>(CaptureSize.Y);
	}
	else
	{
		// if the viewport is taller than it is wide
		XAxisMultiplier = CaptureSize.Y / (float)CaptureSize.X;
		YAxisMultiplier = 1.0f;
	}
	FMatrix ProjectionMatrix = FReversedZPerspectiveMatrix (
	   FOV /2,
	   FOV /2,
	   XAxisMultiplier,
	   YAxisMultiplier,
	   GNearClippingPlane,
	   GNearClippingPlane
	   );

	const FMatrix InverseViewMatrix = ViewMatrix.InverseFast();
	const FMatrix InvProjectionMatrix = ProjectionMatrix.Inverse();

	const FIntRect ViewRect = FIntRect(0, 0, CaptureSize.X, CaptureSize.Y);

	FSceneView::DeprojectScreenToWorld(ScreenPos, ViewRect,
		InverseViewMatrix,
		InvProjectionMatrix,
		OutWorldOrigin,
		OutWorldDirection);
}

FVector2D UUtilitiesRender::WorldToTextureCoordinates(USceneCaptureComponent2D* SceneCaptureComponent,
	const FVector& WorldPosition)
{
	if (!SceneCaptureComponent || !SceneCaptureComponent->TextureTarget)
	{
		return FVector2D(-1.0f, -1.0f); // Ошибка
	}

	UTextureRenderTarget2D* RenderTarget = SceneCaptureComponent->TextureTarget;

	if (!RenderTarget || RenderTarget->SizeX <= 0 || RenderTarget->SizeY <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid RenderTarget or Texture dimensions."));
		return FVector2D(-1.0f, -1.0f);
	}

	FVector CameraLocation = SceneCaptureComponent->GetComponentLocation();
	FRotator CameraRotation = SceneCaptureComponent->GetComponentRotation();

	FVector ViewCoordinates = UKismetMathLibrary::InverseTransformLocation(
		FTransform(CameraRotation, CameraLocation), 
		WorldPosition
	);

	if (ViewCoordinates.X <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("WorldPosition is behind the camera."));
		return FVector2D(-1.0f, -1.0f);
	}

	float TextureAspectRatio = static_cast<float>(RenderTarget->SizeX) / RenderTarget->SizeY;
	FVector2D ScreenPosition;

	if (SceneCaptureComponent->ProjectionType == ECameraProjectionMode::Perspective)
	{
		float FOV = SceneCaptureComponent->FOVAngle;
		float HalfFOVRad = FMath::DegreesToRadians(FOV / 2.0f);

		ScreenPosition.X = (ViewCoordinates.Y / ViewCoordinates.X) / FMath::Tan(HalfFOVRad);
		ScreenPosition.Y = (ViewCoordinates.Z / ViewCoordinates.X) / FMath::Tan(HalfFOVRad);
		ScreenPosition.Y /= TextureAspectRatio;
	}
	else if (SceneCaptureComponent->ProjectionType == ECameraProjectionMode::Orthographic)
	{
		float OrthoWidth = SceneCaptureComponent->OrthoWidth;
		ScreenPosition.X = ViewCoordinates.Y / (OrthoWidth * 0.5f);
		ScreenPosition.Y = ViewCoordinates.Z / ((OrthoWidth / TextureAspectRatio) * 0.5f);
	}

	ScreenPosition.X = 0.5f + ScreenPosition.X * 0.5f;
	ScreenPosition.Y = 0.5f - ScreenPosition.Y * 0.5f;

	if (ScreenPosition.X < 0.0f || ScreenPosition.X > 1.0f || 
	    ScreenPosition.Y < 0.0f || ScreenPosition.Y > 1.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("ScreenPosition out of texture bounds."));
		return FVector2D(-1.0f, -1.0f);
	}

	FVector2D TextureCoordinates;
	TextureCoordinates.X = ScreenPosition.X * RenderTarget->SizeX;
	TextureCoordinates.Y = ScreenPosition.Y * RenderTarget->SizeY;

	return TextureCoordinates;
}
