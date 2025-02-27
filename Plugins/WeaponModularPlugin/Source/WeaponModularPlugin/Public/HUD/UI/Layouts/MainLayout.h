//  Nublin Studio 2025 All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "../BUIUserWidget.h"
#include "MainLayout.generated.h"

class UWeaponLayout;
/**
 * 
 */
UCLASS()
class WEAPONMODULARPLUGIN_API  UMainLayout : public UBUIUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout")
	TObjectPtr<UWeaponLayout> WeaponLayout;
};
