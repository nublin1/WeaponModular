// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BUIUserWidget.h"
#include "MainLayout.generated.h"

class UWeaponLayout;
/**
 * 
 */
UCLASS()
class WEAPONMODULAR_API UMainLayout : public UBUIUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	TObjectPtr<UWeaponLayout> WeaponLayout;
};
