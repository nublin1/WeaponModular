// Fill out your copyright notice in the Description page of Project Settings.


#include "AInventorySceneRenderer.h"


// Sets default values
AInventorySceneRenderer::AInventorySceneRenderer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInventorySceneRenderer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInventorySceneRenderer::RotateObject(FVector2D Delta)
{
	float RotationSpeed = 35.0f;
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += Delta.X * RotationSpeed * DeltaTime; 
	//NewRotation.Roll -= Delta.Y * RotationSpeed * DeltaTime;

	//NewRotation.Roll = FMath::Clamp(NewRotation.Roll, -80.0f, 80.0f);

	if (ChildComponent)
		if (ChildComponent->GetChildActor())
			ChildComponent->GetChildActor()->AddActorWorldRotation(NewRotation);
}

// Called every frame
void AInventorySceneRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

