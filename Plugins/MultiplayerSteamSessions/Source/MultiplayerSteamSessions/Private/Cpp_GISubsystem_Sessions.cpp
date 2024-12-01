// Fill out your copyright notice in the Description page of Project Settings.


// Engine Includes
#include "Cpp_GISubsystem_Sessions.h"
#include "OnlineSubsystem.h"

UCpp_GISubsystem_Sessions::UCpp_GISubsystem_Sessions() {
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
