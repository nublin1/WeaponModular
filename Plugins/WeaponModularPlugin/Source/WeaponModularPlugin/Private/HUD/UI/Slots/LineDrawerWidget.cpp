//  Nublin Studio 2025 All Rights Reserved.



#include "HUD/UI/Slots/LineDrawerWidget.h"

#include "MovieSceneTracksPropertyTypes.h"
#include "Math/Color.h"

ULineDrawerWidget::ULineDrawerWidget()
{
}

void ULineDrawerWidget::ClearLinesToDraw()
{
	LineDrawers.Empty();
}

bool ULineDrawerWidget::AddLineToDraw(FString LineName, FVector2D StartPoint, FVector2D EndPoint, FLinearColor ColorLine, float Thickness)
{
	if (LineName == "")
		return false;

	if (StartPoint == EndPoint)
		return false;
	
	FVector4 Line = FVector4 (StartPoint.X, StartPoint.Y, EndPoint.X, EndPoint.Y);
	FLineDrawInfo LineDrawInfo;
	LineDrawInfo.LineCoordinates = Line;
	LineDrawInfo.LineColor = ColorLine;
	LineDrawInfo.LineThickness = Thickness;

	LineDrawers.Add(LineName, LineDrawInfo);
	
	return true;
}
