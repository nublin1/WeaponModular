//  Nublin Studio 2025 All Rights Reserved.

#include "HUD/UI/Slots/ItemPartIconWidget.h"

#include "Components/Button.h"

UItemPartIconWidget::UItemPartIconWidget()
{
	
}

void UItemPartIconWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (Content_Button)
		Content_Button->OnClicked.AddDynamic(this, &UItemPartIconWidget::Content_ButtonClick);
}

void UItemPartIconWidget::Content_ButtonClick()
{
	if (OnClickedContent_Button.IsBound())
		OnClickedContent_Button.Broadcast(this);
}
