// Fill out your copyright notice in the Description page of Project Settings.


#include "AInventorySceneRenderer.h"

#include "Components/SceneCaptureComponent2D.h"


AInventorySceneRenderer::AInventorySceneRenderer()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	if (CaptureComponent)
	{
		CaptureComponent->SetupAttachment(RootComponent); 
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
	CaptureComponent->AddRelativeLocation(CameraSettings.InitialRelativeLocation);
	if (CameraSettings.bReverseWeapon)
	{
		auto Rotation = ChildComponent->GetComponentRotation();
		ChildComponent->SetRelativeRotation(FRotator(Rotation.Yaw, Rotation.Pitch+180.0f, Rotation.Roll));
	}
	
	CaptureComponent->ProjectionType = CameraSettings.CameraProjectionMode;
	CaptureComponent->OrthoWidth = CameraSettings.OrthoWidth;
	CaptureComponent->FOVAngle = CameraSettings.FOVAngle;
}

void AInventorySceneRenderer::UpdateVisibleComponents()
{
	if (!ChildComponent || !CaptureComponent)
		return;
	
	AActor* ChildActor = ChildComponent->GetChildActor();
	if (!ChildActor)
		return;

	CaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	CaptureComponent->ShowOnlyActors.Reset();
	CaptureComponent->ShowOnlyActors.Add(ChildActor);
	//CaptureComponent->ShowOnlyActorComponents(ChildActor, true);
}

void AInventorySceneRenderer::RotateObject(FVector2D Delta)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
    
	if (ChildComponent && ChildComponent->GetChildActor())
	{
		FRotator ChildRotation = ChildComponent->GetChildActor()->GetActorRotation();
		float DeltaYaw = Delta.X * RotationSettings.RotationSpeed * DeltaTime;
		float DeltaRoll = Delta.Y * RotationSettings.RotationSpeed * DeltaTime;
        
		ChildRotation.Yaw = FMath::Clamp(
			ChildRotation.Yaw + DeltaYaw,
			RotationSettings.MinYaw,
			RotationSettings.MaxYaw
			);
		ChildRotation.Roll = FMath::Clamp(
			ChildRotation.Roll + DeltaRoll,
			RotationSettings.MinRoll,
			RotationSettings.MaxRoll
			);
        
		ChildComponent->GetChildActor()->SetActorRotation(ChildRotation);
	}
}

void AInventorySceneRenderer::ZoomObject(float Delta)
{
	if (!CaptureComponent)
		return;

	if (CaptureComponent->ProjectionType.GetValue() == ECameraProjectionMode::Type::Orthographic)
	{
		float MinOrthoWidth = 50.0f;
		float MaxOrthoWidth = 1000.0f;
		
		float NewOrthoWidth = CaptureComponent->OrthoWidth - Delta * CameraSettings.CameraOffsetSpeed;
		NewOrthoWidth = FMath::Clamp(NewOrthoWidth, MinOrthoWidth, MaxOrthoWidth);
		CaptureComponent->OrthoWidth = NewOrthoWidth;
	}
	else
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		auto CurrentLocation = CaptureComponent->GetComponentLocation();
		FVector NewLocation = CurrentLocation + CaptureComponent->GetForwardVector() * Delta * CameraSettings.CameraOffsetSpeed * DeltaTime;

		FVector StartLocation = CaptureComponent->GetComponentLocation();
		FVector OffsetVector = NewLocation - StartLocation;
		float Distance = OffsetVector.Size();
		float ClampedDistance = FMath::Clamp(Distance, FMath::Abs(CameraSettings.MinOffsetDistance), FMath::Abs(CameraSettings.MaxOffsetDistance));
		FVector ClampedOffsetVector = OffsetVector.GetSafeNormal() * ClampedDistance;
	
		CaptureComponent->SetWorldLocation(NewLocation);
	}
}

void AInventorySceneRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
