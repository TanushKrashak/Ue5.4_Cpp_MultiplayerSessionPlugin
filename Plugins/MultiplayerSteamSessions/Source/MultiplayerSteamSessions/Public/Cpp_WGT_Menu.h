// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cpp_WGT_Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSTEAMSESSIONS_API UCpp_WGT_Menu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void InitializeMenu();
};
