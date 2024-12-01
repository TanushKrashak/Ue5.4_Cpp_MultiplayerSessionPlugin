// Fill out your copyright notice in the Description page of Project Settings.


// Engine Includes
#include "Cpp_GISubsystem_Sessions.h"
#include "OnlineSubsystem.h"

UCpp_GISubsystem_Sessions::UCpp_GISubsystem_Sessions() :
	// Bind All Delegates For Online Subsystem Sessions
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UCpp_GISubsystem_Sessions::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UCpp_GISubsystem_Sessions::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UCpp_GISubsystem_Sessions::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &UCpp_GISubsystem_Sessions::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &UCpp_GISubsystem_Sessions::OnStartSessionComplete)) {
	// Get the online subsystem
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get()) {
		// Get the session interface
		SessionInterface = OnlineSubsystem->GetSessionInterface();
	}
}

void UCpp_GISubsystem_Sessions::CreateSession(const int32 NumPublicConnections, const FString& MatchType) {
	
}
void UCpp_GISubsystem_Sessions::FindSessions(const int32 MaxSearchResults) {
	
}
void UCpp_GISubsystem_Sessions::JoinSession(const FOnlineSessionSearchResult& SearchResult) {
	
}
void UCpp_GISubsystem_Sessions::DestroySession() {
	
}
void UCpp_GISubsystem_Sessions::StartSession() {
	
}

void UCpp_GISubsystem_Sessions::OnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful) const {
	
}
void UCpp_GISubsystem_Sessions::OnFindSessionsComplete(const bool bWasSuccessful) const {

}
void UCpp_GISubsystem_Sessions::OnJoinSessionComplete(const FName SessionName, EOnJoinSessionCompleteResult::Type Result) const {

}
void UCpp_GISubsystem_Sessions::OnDestroySessionComplete(const FName SessionName, const bool bWasSuccessful) const {

}
void UCpp_GISubsystem_Sessions::OnStartSessionComplete(const FName SessionName, const bool bWasSuccessful) const {

}
