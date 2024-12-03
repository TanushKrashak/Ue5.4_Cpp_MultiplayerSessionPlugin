// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cpp_WGT_Menu.generated.h"

// Forward Declarations
class UCpp_GISubsystem_Sessions;
class UButton;

/**
 * 
 */
UCLASS()
class MULTIPLAYERSTEAMSESSIONS_API UCpp_WGT_Menu : public UUserWidget
{
	GENERATED_BODY()

public:
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void InitializeMenu(const int32 PublicConnectionsCount = 4, const FString& InMatchType = "FreeForAll");

protected:
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	virtual bool Initialize() override;

private:
	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Host;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Join;

	// Subsystem designed to handle session functionality
	UPROPERTY()
	UCpp_GISubsystem_Sessions* MultiplayerSessionSubsystem = nullptr;

	int32 NumPublicConnections{4};
	FString MatchType{"FreeForAll"};
	
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();

	UFUNCTION()
	void DestroyWidget();
};
