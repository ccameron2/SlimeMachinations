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

	UPROPERTY()
		class AThirdPersonCharacter* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShopShown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CursorManualUnlock = false;

private:

	virtual void CallForward(float Value);
	virtual void CallTurn(float Value);
	virtual void CallFire();
	virtual void CallStrafe(float Value);
	virtual void CallJump();
	virtual void CallLookUp(float Value);
	virtual void OpenShop();
	virtual void UnlockCursor();
	virtual void CallToggleSprint();
	


	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> UIClass;

	UPROPERTY()
		UUserWidget* HUDWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> ShopClass;

	UPROPERTY()
		UUserWidget* ShopWidget;


};
