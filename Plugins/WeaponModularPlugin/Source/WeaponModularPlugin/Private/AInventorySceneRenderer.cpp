//  Nublin Studio 2025 All Rights Reserved.


#include "AInventorySceneRenderer.h"
#include "Engine/World.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

AInventorySceneRenderer::AInventorySceneRenderer()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	if (CaptureComponent)
	{
		CaptureComponent.Get()->SetupAttachment(RootComponent); 
	}
	
	ChildComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildComponent"));
	if (ChildComponent)
	{
		ChildComponent->SetupAttachment(RootComponent); 
	}
}

void AInventorySceneRenderer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AInventorySceneRenderer::BeginPlay()
{
	Super::BeginPlay();

	UpdateVisibleComponents();
}

void AInventorySceneRenderer::Initialize()
{
	CaptureComponent.Get()->AddRelativeLocation(CameraSettings.InitialRelativeLocation);
	if (CameraSettings.bReverseWeapon)
	{
		auto Rotation = ChildComponent->GetComponentRotation();
		ChildComponent->SetRelativeRotation(FRotator(Rotation.Yaw, Rotation.Pitch+180.0f, Rotation.Roll));
	}
	
	CaptureComponent.Get()->ProjectionType = CameraSettings.CameraProjectionMode;
	CaptureComponent.Get()->OrthoWidth = CameraSettings.OrthoWidth;
	CaptureComponent.Get()->FOVAngle = CameraSettings.FOVAngle;
}

void AInventorySceneRenderer::UpdateVisibleComponents()
{
	if (!ChildComponent || !CaptureComponent)
		return;
	
	AActor* ChildActor = ChildComponent->GetChildActor();
	if (!ChildActor)
		return;

	CaptureComponent.Get()->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	CaptureComponent.Get()->ShowOnlyActors.Reset();
	CaptureComponent.Get()->ShowOnlyActors.Add(ChildActor);
	//CaptureComponent->ShowOnlyActorComponents(ChildActor, true);
}

void AInventorySceneRenderer::RotateObject(FVector2D Delta)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (ChildComponent && ChildComponent->GetChildActor())
	{
		AActor* ChildActor = ChildComponent->GetChildActor();
		ChildComponent->AddRelativeLocation(-Relative);
		
		FRotator ChildRotation = ChildActor->GetActorRotation();
		ChildRotation.Yaw += Delta.X * RotationSettings.RotationSpeed * DeltaTime;
		ChildRotation.Pitch += Delta.Y * RotationSettings.RotationSpeed * DeltaTime;

		auto WrapAngle = [](float Angle, float Min, float Max) {
			const float Range = Max - Min;
			if (Range >= 360.0f) {
				Angle = FMath::Fmod(Angle - Min, 360.0f);
				return Angle + (Angle < 0 ? 360.0f : 0) + Min;
			}
			return FMath::Clamp(Angle, Min, Max);
		};

		// 
		ChildRotation.Yaw = WrapAngle(ChildRotation.Yaw, RotationSettings.XAxisMinClamp, RotationSettings.XAxisMaxClamp);
		ChildRotation.Pitch = WrapAngle(ChildRotation.Pitch, RotationSettings.YAxisMinClamp, RotationSettings.YAxisMaxClamp);
		
		ChildComponent->SetRelativeRotation(ChildRotation);
		ChildComponent->AddRelativeLocation(Relative);
	}
}

void AInventorySceneRenderer::ZoomObject(float Delta)
{
	if (!CaptureComponent)
		return;

	if (CaptureComponent.Get()->ProjectionType.GetValue() == ECameraProjectionMode::Type::Orthographic)
	{
		float MinOrthoWidth = 50.0f;
		float MaxOrthoWidth = 1000.0f;
		
		float NewOrthoWidth = CaptureComponent->OrthoWidth - Delta * CameraSettings.CameraOffsetSpeed;
		NewOrthoWidth = FMath::Clamp(NewOrthoWidth, MinOrthoWidth, MaxOrthoWidth);
		CaptureComponent.Get()->OrthoWidth = NewOrthoWidth;
	}
	else
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		auto CurrentLocation = CaptureComponent.Get()->GetComponentLocation();
		FVector NewLocation = CurrentLocation + CaptureComponent.Get()->GetForwardVector() * Delta * CameraSettings.CameraOffsetSpeed * DeltaTime;

		FVector StartLocation = CaptureComponent.Get()->GetComponentLocation();
		FVector OffsetVector = NewLocation - StartLocation;
		float Distance = OffsetVector.Size();
		float ClampedDistance = FMath::Clamp(Distance, FMath::Abs(CameraSettings.MinOffsetDistance), FMath::Abs(CameraSettings.MaxOffsetDistance));
		FVector ClampedOffsetVector = OffsetVector.GetSafeNormal() * ClampedDistance;
	
		CaptureComponent.Get()->SetWorldLocation(NewLocation);
	}
}

void AInventorySceneRenderer::CenterChildComponent()
{
	if (!ChildComponent)
		return;

	AActor* ChildActor = ChildComponent->GetChildActor();
	if (USkeletalMeshComponent* SkeletalMeshComponent = ChildActor->FindComponentByClass<USkeletalMeshComponent>())
	{
		FRotator OriginalRotation = ChildActor->GetActorRotation();
		ChildActor->SetActorRotation(FRotator::ZeroRotator);
		
		FBox TotalBounds(ForceInit);
		TotalBounds += SkeletalMeshComponent->Bounds.GetBox();
		TArray<UStaticMeshComponent*> StaticMeshComponents;
		ChildActor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

		for (UStaticMeshComponent* StaticMeshComponent : StaticMeshComponents)
		{
			if (StaticMeshComponent && StaticMeshComponent->IsVisible())
			{
				TotalBounds += StaticMeshComponent->Bounds.GetBox();
			}
		}

		if (!TotalBounds.IsValid)
			return;

		ChildActor->SetActorRotation(OriginalRotation);
		Relative = ChildActor->GetActorLocation() - TotalBounds.GetCenter();
		ChildActor->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		ChildActor->AddActorLocalOffset(Relative);
	}
}

void AInventorySceneRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
