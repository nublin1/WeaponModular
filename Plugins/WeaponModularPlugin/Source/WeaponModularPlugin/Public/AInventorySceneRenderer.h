// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Settings/Settings.h"
#include "AInventorySceneRenderer.generated.h"


UCLASS(Blueprintable)
class WEAPONMODULARPLUGIN_API AInventorySceneRenderer : public AActor
{
	GENERATED_BODY()
	
public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UChildActorComponent> ChildComponent;
	
	//====================================================================
	// FUNCTIONS
	//====================================================================
	AInventorySceneRenderer();
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION(BlueprintCallable)
	void Initialize();
	UFUNCTION(BlueprintCallable)
	void UpdateVisibleComponents();

	// Setters
	void SetRotationSettings(const FRotationSettings NewSettings) {RotationSettings = NewSettings;}
	void SetCameraSettings(const FCameraSettings& NewSettings) {CameraSettings = NewSettings;}
	
protected:
	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void RotateObject(FVector2D Delta);
	UFUNCTION(BlueprintCallable)
	virtual void ZoomObject(float Delta);
	UFUNCTION(BlueprintCallable)
	void CenterChildComponent();

	UPROPERTY()
	FRotationSettings RotationSettings;
	UPROPERTY()
	FCameraSettings CameraSettings;

	UPROPERTY()
	FVector Relative;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
