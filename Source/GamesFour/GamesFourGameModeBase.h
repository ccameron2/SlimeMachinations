// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ResourcePickup.h"
#include "SlimeEnemy.h"
#include "SlimeSpawner.h"
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
	
	// Class for slime spawner
	UPROPERTY(EditAnywhere)
		TSubclassOf<ASlimeSpawner> SlimeSpawnerClass;

	// List of slime spawners
	UPROPERTY(VisibleAnywhere)
		TArray<ASlimeSpawner*> SlimeSpawnerList;

	// Cooldown between spawning new slime spawners
	UPROPERTY(EditAnywhere)
		float SlimeSpawnerCooldown = 30.0f;

	// Range that spawners can be spawned in in both directions
	UPROPERTY(EditAnywhere)
		int SpawnRange = 3000;

	// Function called when timer runs out
	UFUNCTION()
		void TimeUp();

public:

	// Delete all slime spawners
	UFUNCTION()
		void ClearSlimeSpawners();

	// Slimes killed for combo
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SlimeKills = 0;

	// Get slimes killed for combo
	UFUNCTION()
		int GetSlimeKills();

	// Shop gold pool
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ShopGold = 0;

	// Get shop gold pool amount
	UFUNCTION()
		int GetShopGold();
};
