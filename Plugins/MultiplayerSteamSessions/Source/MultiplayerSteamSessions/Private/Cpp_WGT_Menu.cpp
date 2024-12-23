// Fill out your copyright notice in the Description page of Project Settings.


#include "Cpp_WGT_Menu.h"
#include "Components/Button.h"
#include "Cpp_GISubsystem_Sessions.h"
#include "OnlineSessionSettings.h"

void UCpp_WGT_Menu::InitializeMenu(const int32 PublicConnectionsCount, const FString& InMatchType, const FString& LobbyPath) {
	// Set the path to the lobby map + making it listen server
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
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

	// Get the GameInstance and the Subsystem
	if (const UGameInstance* GameInstance = GetGameInstance()) {
		if (const auto Subsystem = GameInstance->GetSubsystem<UCpp_GISubsystem_Sessions>()) {
			MultiplayerSessionSubsystem = Subsystem;
		}
	}

	// Bind the custom delegate to the OnCreateSession function
	if (MultiplayerSessionSubsystem) {
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &UCpp_WGT_Menu::OnCreateSession);
		MultiplayerSessionSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &UCpp_WGT_Menu::OnFindSessions);
		MultiplayerSessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &UCpp_WGT_Menu::OnJoinSession);
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UCpp_WGT_Menu::OnDestroySession);
		MultiplayerSessionSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &UCpp_WGT_Menu::OnStartSession);
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

void UCpp_WGT_Menu::OnCreateSession(const bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, bWasSuccessful ? "Session Created!" : "Session Creation Failed!");
	if (bWasSuccessful) {
		if (UWorld* World = GetWorld()) {
			World->ServerTravel(PathToLobby);
			DestroyWidget();
		}
	}
	else {
		BTN_Host->SetIsEnabled(true);
	}
}
void UCpp_WGT_Menu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, const bool bWasSuccessful) {
	if (!bWasSuccessful || !MultiplayerSessionSubsystem) {
		return;
	}
	
	// Go through the results and check if they match the desired match type
	for (auto Result : SessionResults) {
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType) {
			MultiplayerSessionSubsystem->JoinSession(Result);
			return;
		}
	}
	// If no session was found, enable the join button
	if (!bWasSuccessful || SessionResults.Num() == 0) {
		BTN_Join->SetIsEnabled(true);
	}
}
void UCpp_WGT_Menu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result) {
	// If the join was successful, travel to the game map
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get()) {
		if (const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface(); SessionInterface.IsValid()) {
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController()) {
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
	// If the join failed, enable the join button
	if (Result != EOnJoinSessionCompleteResult::Success) {
		BTN_Join->SetIsEnabled(true);
	}
}
void UCpp_WGT_Menu::OnDestroySession(const bool bWasSuccessful) {

}
void UCpp_WGT_Menu::OnStartSession(const bool bWasSuccessful) {

}

void UCpp_WGT_Menu::OnHostClicked() {
	BTN_Host->SetIsEnabled(false);
	
	if (MultiplayerSessionSubsystem) {
		MultiplayerSessionSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}
void UCpp_WGT_Menu::OnJoinClicked() {
	BTN_Join->SetIsEnabled(false);
	
	if (MultiplayerSessionSubsystem) {
		MultiplayerSessionSubsystem->FindSessions(10000);
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
