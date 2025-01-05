// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/Slots/LineDrawerWidget.h"

ULineDrawerWidget::ULineDrawerWidget()
{
}

bool ULineDrawerWidget::AddLineToDraw(FString LineName, FVector2D StartPoint, FVector2D EndPoint)
{
	if (LineName == "")
		return false;

	if (StartPoint == EndPoint)
		return false;

	FVector4 Line = FVector4 (StartPoint.X, StartPoint.Y, EndPoint.X, EndPoint.Y);
	LineDrawers.Add(LineName, Line);
	
	return true;
}
