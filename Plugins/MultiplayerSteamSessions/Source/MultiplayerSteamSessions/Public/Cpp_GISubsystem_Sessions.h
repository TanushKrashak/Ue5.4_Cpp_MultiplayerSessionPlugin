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

	
private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	IOnlineSessionPtr SessionInterface;
	
	// To Add to the OnlineSessionInterface Delegate List
	// We'll bind our MultiplayerSessionSubsystem internal callbacks to these. 
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	
};
