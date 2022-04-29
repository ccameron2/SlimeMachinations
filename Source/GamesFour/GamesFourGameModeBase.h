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

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASlimeSpawner> SlimeSpawnerClass;

	UPROPERTY(VisibleAnywhere)
		TArray<ASlimeSpawner*> SlimeSpawnerList;

	UPROPERTY(EditAnywhere)
		float SlimeSpawnerCooldown = 30.0f;

	UPROPERTY(EditAnywhere)
		int SlimeLimit = 1000;

	UPROPERTY(EditAnywhere)
		int SpawnRange = 3000;

	UFUNCTION()
		void TimeUp();

public:

	UFUNCTION()
		void ClearSlimeSpawners();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SlimeKills = 0;

	UFUNCTION()
		int GetSlimeKills();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ShopGold = 0;

	UFUNCTION()
		int GetShopGold();
};
