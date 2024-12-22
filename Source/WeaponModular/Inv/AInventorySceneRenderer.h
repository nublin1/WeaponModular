// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AInventorySceneRenderer.generated.h"



UCLASS()
class WEAPONMODULAR_API AInventorySceneRenderer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventorySceneRenderer();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UChildActorComponent> ChildComponent;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void RotateObject(FVector2D Delta);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
