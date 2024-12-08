
// Engine Includes
#include "Cpp_GISubsystem_Sessions.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"

UCpp_GISubsystem_Sessions::UCpp_GISubsystem_Sessions() :
	// Bind All Delegates For Online Subsystem Sessions
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UCpp_GISubsystem_Sessions::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UCpp_GISubsystem_Sessions::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UCpp_GISubsystem_Sessions::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &UCpp_GISubsystem_Sessions::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &UCpp_GISubsystem_Sessions::OnStartSessionComplete))
{
	
	// Get the online subsystem
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get()) {
		// Get the session interface
		SessionInterface = OnlineSubsystem->GetSessionInterface();
	}
}

void UCpp_GISubsystem_Sessions::CreateSession(const int32 NumPublicConnections, const FString& MatchType) {
	if (!SessionInterface.IsValid()) {
		return;
	}
	// Destroy the existing session if it exists
	if (auto ExistingSession = SessionInterface.Pin()->GetNamedSession(NAME_GameSession); ExistingSession != nullptr) {
		SessionInterface.Pin()->DestroySession(NAME_GameSession);
	}
	// Store the delegate so that it can be removed later. Also, bind the delegate to the internal callback
	CreateSessionCompleteDelegateHandle = SessionInterface.Pin()->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	// This MatchType is a custom key that we can use to filter sessions
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->BuildUniqueId = 1;

	// Create a new session
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface.Pin()->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings)) {
		// If the session creation fails, remove the delegate
		SessionInterface.Pin()->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// Call the custom delegate
		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}
}
void UCpp_GISubsystem_Sessions::FindSessions(const int32 MaxSearchResults) {
	if (!SessionInterface.IsValid()) {
		return;
	}

	FindSessionsCompleteDelegateHandle = SessionInterface.Pin()->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	// Set up the session search
	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// Find the sessions
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface.Pin()->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef())) {
		// Session search failed, remove the delegate
		SessionInterface.Pin()->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		// Let the menu know that the search failed
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}
void UCpp_GISubsystem_Sessions::JoinSession(const FOnlineSessionSearchResult& SearchResult) {
	if (!SessionInterface.IsValid()) {
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface.Pin()->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface.Pin()->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SearchResult)) {
		// If the join fails, remove the delegate
		SessionInterface.Pin()->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		// Let the menu know that the join failed
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}
void UCpp_GISubsystem_Sessions::DestroySession() {
	
}
void UCpp_GISubsystem_Sessions::StartSession() {
	
}

void UCpp_GISubsystem_Sessions::OnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful) {
	// Clear the delegate
	if (SessionInterface.IsValid()) {
		SessionInterface.Pin()->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}
void UCpp_GISubsystem_Sessions::OnFindSessionsComplete(const bool bWasSuccessful) {
	if (SessionInterface.IsValid()) {
		SessionInterface.Pin()->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	// If the search was successful but there are no results, Let the menu know that it failed
	if (LastSessionSearch->SearchResults.Num() == 0) {
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	
	// Let the menu know that the search is complete
	MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}
void UCpp_GISubsystem_Sessions::OnJoinSessionComplete(const FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	// Clear the delegate
	if (SessionInterface.IsValid()) {
		SessionInterface.Pin()->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	MultiplayerOnJoinSessionComplete.Broadcast(Result);
}
void UCpp_GISubsystem_Sessions::OnDestroySessionComplete(const FName SessionName, const bool bWasSuccessful) const {

}
void UCpp_GISubsystem_Sessions::OnStartSessionComplete(const FName SessionName, const bool bWasSuccessful) const {

}
