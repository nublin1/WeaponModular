//  Nublin Studio 2025 All Rights Reserved.

#include "HUD/UI/BUIUserWidget.h"

void UBUIUserWidget::GeometryReady()
{
	if (OnGeometryReady.IsBound())
		OnGeometryReady.Broadcast(this);
}

void UBUIUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}
