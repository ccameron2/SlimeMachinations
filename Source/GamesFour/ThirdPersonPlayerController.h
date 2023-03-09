// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ThirdPersonCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThirdPersonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMESFOUR_API AThirdPersonPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	virtual void SetupInputComponent();

	virtual void Tick(float DeltaTime) override;

	// Reference to the player's pawn
	UPROPERTY()
		class AThirdPersonCharacter* PlayerPawn;

	// Is the shop currently on the screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShopShown = false;

	// Has the cursor been unlocked
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CursorManualUnlock = false;

private:

	// Functions to call pawn functions through input
	virtual void CallForward(float Value);
	virtual void CallTurn(float Value);
	virtual void CallFire();
	virtual void CallStrafe(float Value);
	virtual void CallJump();
	virtual void CallLookUp(float Value);
	virtual void CallToggleSprint();

	// Invert the Y axis of the camera
	virtual void InvertMouse();

	// Open the shop screen
	virtual void OpenShop();

	// Unlock the cursor
	virtual void UnlockCursor();

	// Has the mouse been inverted
	bool MouseInverted = false;

	// Class for the HUD
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> UIClass;

	// Widget for the HUD
	UPROPERTY()
		UUserWidget* HUDWidget;

	// Class for the Shop
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> ShopClass;

	// Widget for the shop
	UPROPERTY()
		UUserWidget* ShopWidget;


};
