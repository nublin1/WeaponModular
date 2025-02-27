//  Nublin Studio 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BUIUserWidget.generated.h"

#pragma region Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGeometryReady, UBUIUserWidget*, BUIUserWidget);
#pragma endregion

UCLASS()
class WEAPONMODULARPLUGIN_API  UBUIUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//====================================================================
	// PROPERTIES AND VARIABLES
	//====================================================================

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnGeometryReady OnGeometryReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bIsGeometryReady = false;

protected:
	//====================================================================
	// FUNCTIONS
	//====================================================================

	UFUNCTION(Category = "Actions")
	virtual void GeometryReady();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
