// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponModular/Public/HUD/UI/BUIUserWidget.h"

void UBUIUserWidget::GeometryReady()
{
	if (OnGeometryReady.IsBound())
		OnGeometryReady.Broadcast(this);
}

void UBUIUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}
