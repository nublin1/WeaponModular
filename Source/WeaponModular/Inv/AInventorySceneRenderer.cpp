// Fill out your copyright notice in the Description page of Project Settings.


#include "AInventorySceneRenderer.h"

#include "Components/SceneCaptureComponent2D.h"


// Sets default values
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

void AInventorySceneRenderer::BeginPlay()
{
	Super::BeginPlay();

	UpdateVisibleComponents();
}

void AInventorySceneRenderer::RotateObject(FVector2D Delta)
{
	float RotationSpeed = 35.0f;
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += Delta.X * RotationSpeed * DeltaTime; 

	if (ChildComponent)
		if (ChildComponent->GetChildActor())
			ChildComponent->GetChildActor()->AddActorWorldRotation(NewRotation);
}

void AInventorySceneRenderer::ZoomObject(float Delta)
{
	if (!CaptureComponent)
		return;

	float ZoomSpeed = 10.0f;

	if (CaptureComponent->ProjectionType.GetValue() == ECameraProjectionMode::Type::Orthographic)
	{
		float MinOrthoWidth = 50.0f;
		float MaxOrthoWidth = 1000.0f;
		
		float NewOrthoWidth = CaptureComponent->OrthoWidth - Delta * ZoomSpeed;
		NewOrthoWidth = FMath::Clamp(NewOrthoWidth, MinOrthoWidth, MaxOrthoWidth);
		CaptureComponent->OrthoWidth = NewOrthoWidth;
	}
	else
	{
		float MinZoomDistance = -30.0f;
		float MaxZoomDistance = -1000.0f;
		float DeltaTime = GetWorld()->GetDeltaSeconds();

		auto CurrentLocation = CaptureComponent->GetComponentLocation();
		FVector NewLocation = CurrentLocation + CaptureComponent->GetForwardVector() * Delta * ZoomSpeed * DeltaTime;
	
		CaptureComponent->SetWorldLocation(NewLocation);
	}
}

void AInventorySceneRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
