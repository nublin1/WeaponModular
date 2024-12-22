// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USceneMarker;

UCLASS()
class WEAPONMODULAR_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:		
	AWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Markers")
	//TObjectPtr<USceneMarker> BarrelMarker;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<USceneMarker>> WeaponMarkers;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};
