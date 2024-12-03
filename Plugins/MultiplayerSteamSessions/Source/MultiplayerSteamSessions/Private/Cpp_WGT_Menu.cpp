// Fill out your copyright notice in the Description page of Project Settings.


#include "Cpp_WGT_Menu.h"

void UCpp_WGT_Menu::InitializeMenu() {
	// Add the widget to the viewport & set it to visible & focusable
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	// Show the mouse cursor & set the input mode to UI only
	if (UWorld* World = GetWorld()) {
		if (APlayerController* PlayerController = World->GetFirstPlayerController()) {
			PlayerController->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
		}
	}
	
}
