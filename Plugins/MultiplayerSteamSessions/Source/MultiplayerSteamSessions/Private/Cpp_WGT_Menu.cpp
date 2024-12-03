// Fill out your copyright notice in the Description page of Project Settings.


#include "Cpp_WGT_Menu.h"
#include "Components/Button.h"

void UCpp_WGT_Menu::InitializeMenu() {
	// Add the widget to the viewport & set it to visible & focusable
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

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
bool UCpp_WGT_Menu::Initialize() {
	if (!Super::Initialize()) {
		return false;
	}
	
	// Bind the buttons to their respective OnClicked functions
	if (BTN_Host) {
		BTN_Host->OnClicked.AddDynamic(this, &UCpp_WGT_Menu::OnHostClicked);
	}
	if (BTN_Join) {
		BTN_Join->OnClicked.AddDynamic(this, &UCpp_WGT_Menu::OnJoinClicked);
	}
	return true;
}

void UCpp_WGT_Menu::OnHostClicked() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Host Clicked!"));
}
void UCpp_WGT_Menu::OnJoinClicked() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Join Clicked!"));
}
