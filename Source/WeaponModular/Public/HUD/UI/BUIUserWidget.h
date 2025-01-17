// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BUIUserWidget.generated.h"

#pragma region Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGeometryReady, UBUIUserWidget*, BUIUserWidget);


#pragma endregion


UCLASS()
class WEAPONMODULAR_API UBUIUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGeometryReady OnGeometryReady;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGeometryReady = false;

protected:
	UFUNCTION()
	virtual void GeometryReady();
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;	
};
