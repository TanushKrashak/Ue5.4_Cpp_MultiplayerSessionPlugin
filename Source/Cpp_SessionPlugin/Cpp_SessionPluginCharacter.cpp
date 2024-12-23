// Copyright Epic Games, Inc. All Rights Reserved.

// Game Includes
#include "Cpp_SessionPluginCharacter.h"

// Engine Includes
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// ACpp_SessionPluginCharacter

ACpp_SessionPluginCharacter::ACpp_SessionPluginCharacter():
	// Bind All Delegates For Online Subsystem Sessions
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)) {
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	// Online Subsystem
	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld())) {
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		/*if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("OnlineSubsystem found! %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
		}*/
	}
}

void ACpp_SessionPluginCharacter::OpenLobby() {
	if (UWorld* World = GetWorld()) {
		// Travel to the Lobby map as a Listen Server
		World->ServerTravel("/Game/ThirdPerson/Maps/Lobby?listen");
	}
}
void ACpp_SessionPluginCharacter::CallOpenLevel(const FString& Address) {
	UGameplayStatics::OpenLevel(this, FName(*Address));
}
void ACpp_SessionPluginCharacter::CallClientTravel(const FString& Address) {
	if (auto Pc = GetGameInstance()->GetFirstLocalPlayerController()) {
		Pc->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

void ACpp_SessionPluginCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACpp_SessionPluginCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACpp_SessionPluginCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACpp_SessionPluginCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACpp_SessionPluginCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACpp_SessionPluginCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACpp_SessionPluginCharacter::CreateGameSession() {
	// Called when pressing the 1 key
	if (!OnlineSessionInterface.IsValid()) {
		return;
	}
	// Destroy the existing session if it exists
	if (auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession); ExistingSession != nullptr) {
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}
	// Enables the CreateSessionCompleteDelegate delegate to be called
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	// Set up the session settings
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
	SessionSettings->bIsLANMatch = false;
	SessionSettings->NumPublicConnections = 4;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	// This MatchType is a custom key that we can use to filter sessions
	SessionSettings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// Create a new session
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}
void ACpp_SessionPluginCharacter::JoinGameSession() {
	if (!OnlineSessionInterface.IsValid()) {
		return;
	}
	// Enables the FindSessionsCompleteDelegate delegate to be called
	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	
	// Set up the session search
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->bIsLanQuery = false;
	// Making sure Presence is set to true, which means that we will only find sessions that are join-able
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	// Find the sessions
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void ACpp_SessionPluginCharacter::OnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful) const {
	if (bWasSuccessful) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Session %s created successfully!"), *SessionName.ToString()));
		}
	}
	else if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to create session!"));
	}

	// Travel to the Lobby map as a Listen Server
	if (UWorld* World = GetWorld()) {
		World->ServerTravel("/Game/ThirdPerson/Maps/LobbyMap?listen");
	}
}
void ACpp_SessionPluginCharacter::OnFindSessionsComplete(const bool bWasSuccessful) const {
	if (!OnlineSessionInterface.IsValid()) {
		return;
	}
	
	// Go through the search results
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults) {
		FString ID = SearchResult.GetSessionIdStr();
		FString User = SearchResult.Session.OwningUserName;

		FString MatchType;
		SearchResult.Session.SessionSettings.Get(FName("MatchType"), MatchType);
		// Check if the session is a FreeForAll session
		if (MatchType == "FreeForAll") {
			// Print the session ID and the owning user
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Found ID: %s owned by User: %s"), *ID, *User));
			}
			// Enables the JoinSessionComplete delegate to be called
			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

			// Join the session
			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SearchResult);
		}
	}
}
void ACpp_SessionPluginCharacter::OnJoinSessionComplete(const FName SessionName, const EOnJoinSessionCompleteResult::Type Result) const {
	if (!OnlineSessionInterface.IsValid()) {
		return;
	}
	FString Address;
	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address)) {
		// Print the session ID and the owning user
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Found Address: %s"), *Address));
		}

		if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController()) {
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
		
	}
}





