// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USC_WeaponPartAttachmentPoint;


UCLASS()
class WEAPONMODULARPLUGIN_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:		
	AWeapon();

	UFUNCTION(BlueprintCallable)
	void InitAttachPoints();

protected:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<USC_WeaponPartAttachmentPoint>> WeaponMarkers;

	//====================================================================
	// FUNCTIONS
	//====================================================================
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};
