// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/Slots/InventoryItemSlotWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Helpers/SC_WeaponPartAttachmentPoint.h"
#include "HUD/UI/Slots/ItemPartWidget.h"
#include "HUD/UI/Slots/LineDrawerWidget.h"
#include "Utilities/UtilitiesRender.h"
#include "Rendering/DrawElements.h"
#include "AInventorySceneRenderer.h"

void UInventoryItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UItemPartWidget*> ItemPartWidgets;
	TFunction<void(UWidget*, TArray<UItemPartWidget*>&)> GatherChildWidgets = [&GatherChildWidgets, this
		](UWidget* ParentWidget, TArray<UItemPartWidget*>& OutWidgets)
	{
		if (!ParentWidget) return;
		if (UItemPartWidget* ItemPartWidget = Cast<UItemPartWidget>(ParentWidget))
		{
			ItemPartWidget->OnListButtonClick.AddDynamic(this, &UInventoryItemSlotWidget::ListButtonClick);
			OutWidgets.Add(ItemPartWidget);
		}

		if (UPanelWidget* Panel = Cast<UPanelWidget>(ParentWidget))
		{
			for (int32 i = 0; i < Panel->GetChildrenCount(); ++i)
			{
				GatherChildWidgets(Panel->GetChildAt(i), OutWidgets);
			}
		}
	};

	if (ContentPanel)
	{
		GatherChildWidgets(ContentPanel.Get(), ItemPartWidgets);
	}

	if (ItemPartWidgets.Num() > 0)
	{
		PartWidgets = ItemPartWidgets;
	}
}

void UInventoryItemSlotWidget::InitializeWidgetPositions()
{
	ItemWidgetsArray.Empty();
	for (auto ItemPartWidget : PartWidgets)
	{
		if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(ItemPartWidget->Slot))
		{
			FItemsWidgetSlot ItemsWidgetSlot;
			ItemsWidgetSlot.SlotPosition = CanvasPanelSlot->GetPosition();
			ItemsWidgetSlot.ItemPartWidgetLinked = ItemPartWidget;
			ItemWidgetsArray.Add(ItemsWidgetSlot);
		}
	}
}

void UInventoryItemSlotWidget::RecalculateLinesToDraw()
{
	if (!WBP_LineDrawer)
		return;
	
	for (auto PWidget : PartWidgets)
	{
		CalculateLineToDraw(PWidget);
	}
}

void UInventoryItemSlotWidget::ComparisonAndUpdateItemPartWidget(UItemPartWidget* Widget,
	USC_WeaponPartAttachmentPoint* AttachmentPoint)
{
	if (!Widget || !AttachmentPoint)
		return;

	bool Result = UWeaponPartDataUtilities::AreWeaponPartPropertiesEqual(
		Widget->GetWidgetWeaponPartType(),
		AttachmentPoint->WeaponPointType
	);

	if (!Result)
		return;

	Widget->SetIconMaterial(UISettings.IconMaterial);
	Widget->SetTargetMarkerLinked(AttachmentPoint);
	Widget->UpdateVisual();

	ForceLayoutPrepass();
	CreateAndPositionListWidget(Widget);
}

void UInventoryItemSlotWidget::AddItemPartWidget(USC_WeaponPartAttachmentPoint* AttachmentPoint)
{
	if (!LinkedSceneRenderer)
		return;
	auto PosInScreen = CalculateCoordinates(LinkedSceneRenderer->CaptureComponent, AttachmentPoint->GetComponentLocation());
	auto Index = FindIndexOfClosestAvaiableWidgetPosition(PosInScreen);
	if (Index < 0)
		return;

	if (TObjectPtr<UItemPartWidget> NewItemWidget = CreateItemPartWidget())
	{
		TObjectPtr<UCanvasPanelSlot> CanvasSlot = ContentPanel->AddChildToCanvas(NewItemWidget);
		if (CanvasSlot)
		{
			CanvasSlot->SetPosition(ItemWidgetsArray[Index].SlotPosition);
			CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f));
			CanvasSlot->SetAutoSize(true);
				
			ItemWidgetsArray[Index].ItemPartWidgetLinked = NewItemWidget;
			NewItemWidget->SetTargetMarkerLinked(AttachmentPoint);
			NewItemWidget->SetWidgetTable(static_cast<UDataTable*>(AttachmentPoint->GetUsableTable()));
			NewItemWidget->SetWidgetWeaponPartType(AttachmentPoint->WeaponPointType);
			NewItemWidget->SetIconMaterial(UISettings.IconMaterial);
			NewItemWidget->SetWeaponPartListWidgetClass(UISettings.WeaponPartListWidgetClass);
			NewItemWidget->UpdateVisual();

			NewItemWidget->OnListButtonClick.AddDynamic(this, &UInventoryItemSlotWidget::ListButtonClick);
			PartWidgets.Add(NewItemWidget);

			CalculateLineToDraw(NewItemWidget);
			if (OnItemPartWidgetAdded.IsBound())
				OnItemPartWidgetAdded.Broadcast(NewItemWidget);

			ForceLayoutPrepass();
			
			CreateAndPositionListWidget(NewItemWidget);
			
		}
	}
}

void UInventoryItemSlotWidget::CalculateItemSlotPositions(FVector2D size)
{
	if (PartWidgets.Num() > 0)
	{
		return;
	}

	FVector2D WidgetSize;

	if (size.IsZero())
		WidgetSize =  GetCachedGeometry().GetLocalSize();
	else
	{
		WidgetSize = size;
	}
	
	const FVector2D Center(WidgetSize.X / 2.0f, WidgetSize.Y / 2.0f);
	for (int32 i = 0; i < UISettings.MaxItemWidgets; ++i)
	{
		FVector2D Position;
		switch (UISettings.WidgetsMethodLocation)
		{
		case EWidgetsMethodLocation::Oval:
			{
				float OvalWidth = WidgetSize.X * 0.8f;
				float OvalHeight = WidgetSize.Y * 0.7f;
				Position = CalculateOvalPosition(i, Center, OvalWidth, OvalHeight);
				break;
			}

		case EWidgetsMethodLocation::Square:
			{
				float SquareWidth = WidgetSize.X * 0.8f;
				float SquareHeight = WidgetSize.Y * 0.8f;
				Position = CalculateSquarePosition(i, Center, SquareWidth, SquareHeight);
				break;
			}
		case EWidgetsMethodLocation::Circle:
			{
				float Radius = WidgetSize.X * 0.8f; 
				Position = CalculateCirclePosition(i, Center, Radius, WidgetSize);
				break;
			}
		}

		FItemsWidgetSlot NewItemsWidgetSlot;
		NewItemsWidgetSlot.SlotPosition = Position;
		ItemWidgetsArray.Add(NewItemsWidgetSlot);
	}
}

TArray<FVector2D> UInventoryItemSlotWidget::GetItemsWidgetPositions()
{
	TArray<FVector2D> Result;
	for (auto Element : ItemWidgetsArray)
	{
		Result.Add(Element.SlotPosition);
	}
	return Result;
}

void UInventoryItemSlotWidget::SetRenderTargetMaterial(UTextureRenderTarget2D* RenderTarget)
{
	if (!UISettings.BaseRenderTargetMaterial) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Base material is null!"));
		return;
	}

	if (!RenderTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("RenderTarget is null!"));
		return;
	}
	
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(UISettings.BaseRenderTargetMaterial , this);
	if (!DynamicMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create dynamic material!"));
		return;
	}
	
	DynamicMaterial->SetTextureParameterValue(FName("RenderTarget"), RenderTarget);
	RT_Image->SetBrushFromMaterial(DynamicMaterial);
	if(auto RT_ImagePanelSlot =  Cast<UCanvasPanelSlot>(RT_Image->Slot))
	{
		RT_ImagePanelSlot->SetSize(FVector2D(RenderTarget->SizeX, RenderTarget->SizeY));
	}
	
}

FVector2D UInventoryItemSlotWidget::CalculateOvalPosition(int32 Index, const FVector2D& Center,
                                                          float OvalWidth, float OvalHeight)
{
	float Angle = 2.0f * PI * Index / UISettings.MaxItemWidgets;
	float X = Center.X + (OvalWidth / 2.0f) * FMath::Cos(Angle);
	float Y = Center.Y + (OvalHeight / 2.0f) * FMath::Sin(Angle);
	float PerspectiveFactor = 1.0f + 0.2f * FMath::Abs(FMath::Sin(Angle));

	//return FVector2D(X, Y);
	return FVector2D(X, Y + PerspectiveFactor);
}

FVector2D UInventoryItemSlotWidget::CalculateSquarePosition(int32 Index, const FVector2D& Center,
                                                            float SquareWidth, float SquareHeight)
{
	float Perimeter = 2.0f * (SquareWidth + SquareHeight);
	float StepLength = Perimeter / UISettings.MaxItemWidgets;

	float CurrentLength = StepLength * Index;
	FVector2D Position;

	if (CurrentLength <= SquareWidth) // Up
	{
		Position = FVector2D(Center.X - SquareWidth / 2.0f + CurrentLength, Center.Y - SquareHeight / 2.0f);
	}
	else if (CurrentLength <= SquareWidth + SquareHeight) // Right
	{
		CurrentLength -= SquareWidth;
		Position = FVector2D(Center.X + SquareWidth / 2.0f, Center.Y - SquareHeight / 2.0f + CurrentLength);
	}
	else if (CurrentLength <= 2.0f * SquareWidth + SquareHeight) // Down
	{
		CurrentLength -= (SquareWidth + SquareHeight);
		Position = FVector2D(Center.X + SquareWidth / 2.0f - CurrentLength, Center.Y + SquareHeight / 2.0f);
	}
	else // Left
	{
		CurrentLength -= (2.0f * SquareWidth + SquareHeight);
		Position = FVector2D(Center.X - SquareWidth / 2.0f, Center.Y + SquareHeight / 2.0f - CurrentLength);
	}

	return Position;
}

FVector2D UInventoryItemSlotWidget::CalculateCirclePosition(int32 Index, const FVector2D& Center, float Radius,
	const FVector2D& ScreenSize) const
{
	float MaxHorizontalRadius = ScreenSize.X / 2.0f * 0.9f; 
	float MaxVerticalRadius = ScreenSize.Y / 2.0f * 0.9f;  
	
	float HorizontalRadius = FMath::Min(Radius, MaxHorizontalRadius);
	float VerticalRadius = FMath::Min(Radius, MaxVerticalRadius);
	
	float TotalWidgets = UISettings.MaxItemWidgets > 0 ? UISettings.MaxItemWidgets : 1;
	float AngleStep = 360.0f / TotalWidgets;
	
	float Angle = AngleStep * Index;
	float Radians = FMath::DegreesToRadians(Angle);
	
	FVector2D Position = FVector2D(
		Center.X + HorizontalRadius * FMath::Cos(Radians),
		Center.Y + VerticalRadius * FMath::Sin(Radians)
	);

	return Position;
}

UItemPartWidget* UInventoryItemSlotWidget::CreateItemPartWidget()
{
	if (!UISettings.ItemPartWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemPartWidgetClass is not set!"));
		return nullptr;
	}

	return CreateWidget<UItemPartWidget>(GetWorld(), UISettings.ItemPartWidgetClass);
}

int32 UInventoryItemSlotWidget::FindIndexOfClosestAvaiableWidgetPosition(FVector2D ComparedPosition)
{
	int32 Index = -1;
	float ClosestDistance = FLT_MAX;
	
	for (int32 i = 0; i < ItemWidgetsArray.Num(); ++i)
	{
		float Distance = FVector2D::Distance(ComparedPosition, ItemWidgetsArray[i].SlotPosition);
		if (Distance < ClosestDistance &&  ItemWidgetsArray[i].ItemPartWidgetLinked == nullptr)
		{
			ClosestDistance = Distance;
			Index = i;
		}
	}
	
	return Index;
}

FVector2D UInventoryItemSlotWidget::CalculateCoordinates(USceneCaptureComponent2D* SceneCaptureComponent,
                                                         FVector WorldPosition)
{
	auto WPositions = WorldPosition;

	auto Result = UUtilitiesRender::WorldToTextureCoordinates(SceneCaptureComponent, WPositions);

	return FVector2D(Result.X, Result.Y);
}

void UInventoryItemSlotWidget::CalculateLineToDraw(UItemPartWidget* ItemPartWidget)
{
	if (!LinkedSceneRenderer || !LinkedSceneRenderer->CaptureComponent || !WBP_LineDrawer)
		return;
	
	auto AttachPoint = ItemPartWidget->GetTargetMarkerLinked();
	if (!AttachPoint)
		return;

	if (!AttachPoint->GetIsNeedDrawLine())
		return;
	
	auto Name = ItemPartWidget->GetName();
	auto Position = ItemPartWidget->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0,0));
	auto Size = ItemPartWidget->GetMainItemIconWidget()->GetCachedGeometry().GetLocalSize();
	auto StartPoint = Position + FVector2D(Size.X / 2.0f, Size.Y / 2.0f);
	auto EndPoint= CalculateCoordinates(LinkedSceneRenderer->CaptureComponent, AttachPoint->GetComponentLocation());
	auto LineColor = AttachPoint->GetLineColor();
	auto LineThickness = AttachPoint->GetLineThickness();
		
	ItemPartWidget->SetBrushTargetPoint(EndPoint);
	WBP_LineDrawer->AddLineToDraw(Name, StartPoint, EndPoint, LineColor, LineThickness);
}

UWeaponPartListWidget* UInventoryItemSlotWidget::CreateAndPositionListWidget(UItemPartWidget* FromWidget)
{
	auto ListWidget = FromWidget->CreateWeaponPartListWidget();
	if (!ListWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateWeaponPartListWidget is failed!"));
		return nullptr;
	}
	
	ListWidget->SetVisibility(ESlateVisibility::Collapsed);

	FVector2D RenderCurrentPosition = Cast<UCanvasPanelSlot>(FromWidget->Slot)->GetPosition();
	//FVector2D RenderCurrentPosition = FromWidget->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.0f, 0.0f));
	FVector2D LocalSize = FromWidget->GetDesiredSize();
	FVector2D ModPosition = FVector2D(RenderCurrentPosition.X, RenderCurrentPosition.Y + LocalSize.Y);
	FVector2D ScrollSize = ListWidget->GetWeaponPartList_ScrollBox()->GetScrollbarThickness();

	if (TObjectPtr<UCanvasPanelSlot> ListWidgetCanvasSlot = ContentPanel->AddChildToCanvas(ListWidget))
	{
		ListWidgetCanvasSlot->SetPosition(ModPosition);
		ListWidgetCanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f));
		ListWidgetCanvasSlot->SetSize(FVector2D(LocalSize.X + ScrollSize.X, LocalSize.Y));
	}

	// Сохраняем позицию для виджета
	for (auto& ItemWidgetSlot : ItemWidgetsArray)
	{
		if (!ItemWidgetSlot.ItemPartWidgetLinked)
			continue;

		if (ItemWidgetSlot.ItemPartWidgetLinked == FromWidget)
		{
			ItemWidgetSlot.ListSlotPosition = ModPosition;
			break;
		}
	}

	return ListWidget;
}

void UInventoryItemSlotWidget::ListButtonClick(UItemPartWidget* FromWidget)
{
	if (!FromWidget->GetPartListWidget())
	{
		auto ListWidget = CreateAndPositionListWidget(FromWidget);
	}
	else
	{
		if (FromWidget->GetPartListWidget()->GetVisibility() == ESlateVisibility::Visible)
		{
			FromWidget->GetPartListWidget()->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			FromWidget->GetPartListWidget()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventoryItemSlotWidget::UpdateWidgetsPositions()
{
	if (!LinkedSceneRenderer) 
	{
		return;
	}

	const FVector2D WidgetSize = GetCachedGeometry().GetLocalSize();
	const FVector2D Center(WidgetSize.X / 2.0f, WidgetSize.Y / 2.0f);
	
	for (int32 i = 0; i < ItemWidgetsArray.Num(); ++i)
	{
		if (!ItemWidgetsArray[i].ItemPartWidgetLinked)
			continue;
        
		if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(ItemWidgetsArray[i].ItemPartWidgetLinked->Slot))
		{
			FVector2D BasePosition = ItemWidgetsArray[i].SlotPosition;
			FVector2D Radius = FVector2D(Center.X - BasePosition.X, Center.Y - BasePosition.Y);
			float DepthFactor = 0;
            
			FVector2D NewPosition = Calculate3DRotationPosition(
				Radius, 
				RotationAngle,
				DepthFactor, 
				BasePosition
			);
            
			CanvasPanelSlot->SetPosition(NewPosition);

			auto List =ItemWidgetsArray[i].ItemPartWidgetLinked->GetPartListWidget();
			if (!List)
				continue;

			if (UCanvasPanelSlot* CanvasPanelSlotList = Cast<UCanvasPanelSlot>(List->Slot))
			{
				FVector2D BasePositionList = ItemWidgetsArray[i].ListSlotPosition;
				FVector2D RadiusList = FVector2D(Center.X - BasePositionList.X, Center.Y - BasePositionList.Y);
				float DepthFactorList = 0;

				FVector2D ListNewPosition = Calculate3DRotationPosition(
				RadiusList, 
				RotationAngle,
				DepthFactorList, 
				BasePositionList
				);

				CanvasPanelSlotList->SetPosition(ListNewPosition);
			}
		}
	}
}

FVector2D UInventoryItemSlotWidget::Calculate3DRotationPosition(FVector2D Radius, FVector2D AngleOffset, float& OutDepth, FVector2D& BasePosition)
{
	float RadiansX = FMath::DegreesToRadians(AngleOffset.X);
	float OffsetX = Radius.X * FMath::Cos(RadiansX);
	
	OutDepth = FMath::Sin(RadiansX);

	return FVector2D(
		BasePosition.X + Radius.X - OffsetX,
		BasePosition.Y
	);
}

void UInventoryItemSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsGeometryReady && !MyGeometry.GetAbsolutePosition().IsZero()  && !MyGeometry.GetLocalSize().IsZero())
	{
		bIsGeometryReady = true;
		GeometryReady();
	}
	
    RecalculateLinesToDraw(); 
}

FReply UInventoryItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (RotationSettings.bCanRotate && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = true;
		LastMousePosition = InMouseEvent.GetScreenSpacePosition();

		return FReply::Handled().CaptureMouse(TakeWidget());
	}
	return Reply;
}

FReply UInventoryItemSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (bIsDragging && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = false;
		return FReply::Handled().ReleaseMouseCapture();
	}
	return Reply;
}

FReply UInventoryItemSlotWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (bIsDragging)
	{
		FVector2D CurrentMousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D Delta = CurrentMousePosition - LastMousePosition;
		LastMousePosition = CurrentMousePosition;
		//UE_LOG(LogTemp, Warning, TEXT("Delta X: %f, Delta Y: %f"), Delta.X, Delta.Y);

		if (OnMouseMoveDelta.IsBound())
		{
			OnMouseMoveDelta.Broadcast(Delta);
		}

		float DeltaTime = GetWorld()->GetDeltaSeconds();
		RotationAngle.X += Delta.X * RotationSettings.RotationSpeed * DeltaTime;
		RotationAngle.Y += Delta.Y * RotationSettings.RotationSpeed * DeltaTime;
		
		auto WrapAngle = [](float Angle, float Min, float Max) {
			const float Range = Max - Min;
			if (Range >= 360.0f) {
				Angle = FMath::Fmod(Angle - Min, 360.0f);
				return Angle + (Angle < 0 ? 360.0f : 0) + Min;
			}
			return FMath::Clamp(Angle, Min, Max);
		};
		
		RotationAngle.X = WrapAngle(RotationAngle.X, RotationSettings.XAxisMinClamp, RotationSettings.XAxisMaxClamp);
		RotationAngle.Y = WrapAngle(RotationAngle.Y, RotationSettings.YAxisMinClamp, RotationSettings.YAxisMaxClamp);
		
		UpdateWidgetsPositions();
	}

	if (!HasMouseCapture())
	{
		bIsDragging = false;
		return FReply::Unhandled();
	}

	return Reply;
}

FReply UInventoryItemSlotWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	auto EventPath = InMouseEvent.GetEventPath();
	bool bIsCursorOverCurrentWidget = false;
	
	for (auto CurrentWidget : EventPath->Widgets.GetInternalArray())
	{
		if (CurrentWidget.Widget == this->MyWidget)
		{
			bIsCursorOverCurrentWidget = true;
			break;
		}
	}

	if (bIsCursorOverCurrentWidget)
	{
		float WheelDelta = InMouseEvent.GetWheelDelta();
		if (OnMouseWheelDelta.IsBound())
			OnMouseWheelDelta.Broadcast(WheelDelta);

		return FReply::Handled();
	}
	
	return Reply;
}

int32 UInventoryItemSlotWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 RetValue = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	if (PartWidgets.Num() <= 0)
		return RetValue;;

	for (auto& PartWidget : PartWidgets)
	{
		if (PartWidget->GetBrushTargetPoint() != FVector2D(-1, -1))
		{
			if (PartWidget->GetBrushTexture())
			{
				FVector2D Size = PartWidget->GetBrushTargetSize();
				FVector2D Position = PartWidget->GetBrushTargetPoint() - Size * 0.5f; // Центрируем позицию

				// Получаем кисть
				const FSlateBrush& Brush = PartWidget->GetBrushTexture()->Brush;

				// Создаем Layout Transform
				FSlateLayoutTransform LayoutTransform(1.0f, Position);

				// Создаем Paint Geometry
				FPaintGeometry PaintGeometry = AllottedGeometry.ToPaintGeometry(Size, LayoutTransform);

				// Рисуем прямоугольник
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					LayerId,
					PaintGeometry,
					&Brush,
					ESlateDrawEffect::None,
					InWidgetStyle.GetColorAndOpacityTint()
				);

				// Увеличиваем LayerId для следующего элемента
				LayerId++;
			}
		}
	}

	return LayerId;
}

