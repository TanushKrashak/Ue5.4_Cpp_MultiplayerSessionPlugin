// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Cpp_GISubsystem_Sessions.generated.h"

// Custom Delegates For Menu Class
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, const bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);


/**
 * 
 */
UCLASS()
class MULTIPLAYERSTEAMSESSIONS_API UCpp_GISubsystem_Sessions : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================

	// Custom Delegates For Menu Class
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;
	

	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UCpp_GISubsystem_Sessions();
	
	// To Handle Session Functionality. The Menu Class Will Use These 	  
	void CreateSession(const int32 NumPublicConnections, const FString& MatchType);
	void FindSessions(const int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SearchResult);
	void DestroySession();
	void StartSession();

protected:
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	// Internal callbacks for the delegates we'll add to the OnlineSessionInterface
	// don't need to be called from outside
	void OnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful);
	void OnFindSessionsComplete(const bool bWasSuccessful);
	void OnJoinSessionComplete(const FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(const FName SessionName, const bool bWasSuccessful) const;
	void OnStartSessionComplete(const FName SessionName, const bool bWasSuccessful) const;
	
private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================	
	TWeakPtr<IOnlineSession> SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	
	// To Add to the OnlineSessionInterface Delegate List
	// We'll bind our MultiplayerSessionSubsystem internal callbacks to these. 
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;
	
};
