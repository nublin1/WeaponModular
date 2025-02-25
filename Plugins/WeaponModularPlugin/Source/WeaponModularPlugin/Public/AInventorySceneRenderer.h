// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Settings/Settings.h"
#include "Components/SceneCaptureComponent2D.h"
#include "AInventorySceneRenderer.generated.h"

UCLASS(Blueprintable)
class WEAPONMODULARPLUGIN_API AInventorySceneRenderer : public AActor
{
	GENERATED_BODY()
	
public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scene Renderer")
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scene Renderer")
	TObjectPtr<UChildActorComponent> ChildComponent;
	
	//====================================================================
	// FUNCTIONS
	//====================================================================
	AInventorySceneRenderer();
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION(BlueprintCallable, Category="Scene Renderer")
	void Initialize();

	UFUNCTION(BlueprintCallable, Category="Scene Renderer")
	void UpdateVisibleComponents();

	// Setters
	void SetRotationSettings(const FRotationSettings NewSettings) { RotationSettings = NewSettings; }
	void SetCameraSettings(const FCameraSettings& NewSettings) { CameraSettings = NewSettings; }
	
protected:
	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Scene Renderer|Transform")
	virtual void RotateObject(FVector2D Delta);

	UFUNCTION(BlueprintCallable, Category="Scene Renderer|Transform")
	virtual void ZoomObject(float Delta);

	UFUNCTION(BlueprintCallable, Category="Scene Renderer|Transform")
	void CenterChildComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scene Renderer|Settings")
	FRotationSettings RotationSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scene Renderer|Settings")
	FCameraSettings CameraSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scene Renderer|Transform")
	FVector Relative;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
