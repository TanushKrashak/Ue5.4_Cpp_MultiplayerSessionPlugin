// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Cpp_GISubsystem_Sessions.generated.h"

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
	void OnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful) const;
	void OnFindSessionsComplete(const bool bWasSuccessful) const;
	void OnJoinSessionComplete(const FName SessionName, EOnJoinSessionCompleteResult::Type Result) const;
	void OnDestroySessionComplete(const FName SessionName, const bool bWasSuccessful) const;
	void OnStartSessionComplete(const FName SessionName, const bool bWasSuccessful) const;
	
private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	IOnlineSessionPtr SessionInterface;
	
	// To Add to the OnlineSessionInterface Delegate List
	// We'll bind our MultiplayerSessionSubsystem internal callbacks to these. 
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsComplete;
	
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;
	
};
