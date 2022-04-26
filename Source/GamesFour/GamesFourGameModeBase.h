// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ResourcePickup.h"
#include "SlimeEnemy.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GamesFourGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class GAMESFOUR_API AGamesFourGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void StartPlay() override;
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AResourcePickup> ResourceClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ASlimeEnemy> SlimeClass;

};
