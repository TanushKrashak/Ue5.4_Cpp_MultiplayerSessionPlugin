// Fill out your copyright notice in the Description page of Project Settings.


#include "Cpp_WGT_Menu.h"
#include "Components/Button.h"
#include "Cpp_GISubsystem_Sessions.h"

void UCpp_WGT_Menu::InitializeMenu(const int32 PublicConnectionsCount, const FString& InMatchType) {
	NumPublicConnections = PublicConnectionsCount;
	MatchType = InMatchType;

	// Add the widget to the viewport & set it to visible & focusable
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	// Show the mouse cursor & set the input mode to UI only
	if (const UWorld* World = GetWorld()) {
		if (APlayerController* PlayerController = World->GetFirstPlayerController()) {
			PlayerController->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
		}
	}

	if (const UGameInstance* GameInstance = GetGameInstance()) {
		if (const auto Subsystem = GameInstance->GetSubsystem<UCpp_GISubsystem_Sessions>()) {
			MultiplayerSessionSubsystem = Subsystem;
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
	if (MultiplayerSessionSubsystem) {
		MultiplayerSessionSubsystem->CreateSession(NumPublicConnections, MatchType);
		if (UWorld* World = GetWorld()) {
			World->ServerTravel("/Game/ThirdPerson/Maps/LobbyMap?listen");
			DestroyWidget();
		}
	}
}
void UCpp_WGT_Menu::OnJoinClicked() {
	if (MultiplayerSessionSubsystem) {
		
	}
}

void UCpp_WGT_Menu::DestroyWidget() {
	if (const UWorld* World = GetWorld()) {
		if (APlayerController* PlayerController = World->GetFirstPlayerController()) {
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->bShowMouseCursor = false;
		}
	}
	RemoveFromParent();
}
