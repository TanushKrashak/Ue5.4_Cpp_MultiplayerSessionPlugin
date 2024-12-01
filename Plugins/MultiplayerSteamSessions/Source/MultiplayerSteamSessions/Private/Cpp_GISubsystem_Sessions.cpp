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
