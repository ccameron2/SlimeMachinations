// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AThirdPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Get pawn reference
	PlayerPawn = Cast<AThirdPersonCharacter>(GetPawn());

	// If player exists
	if (PlayerPawn)
	{
		// Add HUD to screen
		HUDWidget = CreateWidget(this, UIClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}

		// Add shop to screen and hide
		ShopWidget = CreateWidget(this, ShopClass);
		if (ShopWidget)
		{
			ShopWidget->AddToViewport();
			ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AThirdPersonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	// Bind inputs to assigned functions
	InputComponent->BindAxis("Move Forwards", this, &AThirdPersonPlayerController::CallForward);
	InputComponent->BindAxis("Turn", this, &AThirdPersonPlayerController::CallTurn);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AThirdPersonPlayerController::CallFire);
	InputComponent->BindAxis("Strafe", this, &AThirdPersonPlayerController::CallStrafe);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AThirdPersonPlayerController::CallJump);
	InputComponent->BindAxis("Look Up", this, &AThirdPersonPlayerController::CallLookUp);
	InputComponent->BindAction("Open Shop", EInputEvent::IE_Pressed, this, &AThirdPersonPlayerController::OpenShop);
	InputComponent->BindAction("Unlock Cursor", EInputEvent::IE_Pressed, this, &AThirdPersonPlayerController::UnlockCursor);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AThirdPersonPlayerController::CallToggleSprint);
	InputComponent->BindAction("Invert Mouse", EInputEvent::IE_Pressed, this, &AThirdPersonPlayerController::InvertMouse);

}

void AThirdPersonPlayerController::Tick(float DeltaTime)
{
	if (!PlayerPawn)
	{
		HUDWidget->RemoveFromViewport();
	}
}

void AThirdPersonPlayerController::CallForward(float Value)
{
	if (PlayerPawn)
	{
		PlayerPawn->MoveForward(Value);
	}
}

void AThirdPersonPlayerController::CallTurn(float Value)
{
	if (PlayerPawn)
	{
		PlayerPawn->Turn(Value);
	}
}


void AThirdPersonPlayerController::CallFire()
{
	if (PlayerPawn)
	{
		PlayerPawn->Fire();
	}
}

void AThirdPersonPlayerController::CallStrafe(float Value)
{
	if (PlayerPawn)
	{
		PlayerPawn->Strafe(Value);
	}
}

void AThirdPersonPlayerController::CallJump()
{
	if (PlayerPawn)
	{
		PlayerPawn->Jump();
	}
}

void AThirdPersonPlayerController::CallLookUp(float Value)
{
	if (PlayerPawn)
	{
		// If invert mouse has been enabled
		if (MouseInverted)
		{
			// Flip the input value to negative
			PlayerPawn->LookUp(-Value);
		}
		else
		{
			// Keep the input value positive
			PlayerPawn->LookUp(Value);
		}
	}
}

void AThirdPersonPlayerController::OpenShop()
{
	// If shop is not shown
	if (!ShopShown)
	{
		// Show the shop
		ShopWidget->SetVisibility(ESlateVisibility::Visible);
		ShopShown = true;

		// Pause the game
		UGameplayStatics::SetGamePaused(ShopWidget, ShopShown);

		// Enable the mouse
		bEnableClickEvents = true;
		bShowMouseCursor = true;
	}
	else
	{
		// Hide the shop
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		ShopShown = false;

		// Disable the mouse
		bEnableClickEvents = false;
		bShowMouseCursor = false;
	}
}

void AThirdPersonPlayerController::UnlockCursor()
{
	if (!CursorManualUnlock)
	{
		// Unlock the cursor
		CursorManualUnlock = true;
		bEnableClickEvents = true;
		bShowMouseCursor   = true;

		// Pause the game
		UGameplayStatics::SetGamePaused(HUDWidget, true);
	}
}

void AThirdPersonPlayerController::CallToggleSprint()
{
	if (PlayerPawn)
	{
		PlayerPawn->ToggleSprint();
	}
}

void AThirdPersonPlayerController::InvertMouse()
{
	MouseInverted = !MouseInverted;
}
