// Copyright Epic Games, Inc. All Rights Reserved.


#include "GamesFourGameModeBase.h"

void AGamesFourGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Start timer for spawning new spawners
	FTransform Transform;
	FTimerHandle SpawnerTimer;
	GetWorld()->GetTimerManager().SetTimer(SpawnerTimer, this, &AGamesFourGameModeBase::TimeUp, SlimeSpawnerCooldown, true);
	
	// Get random location for spawner
	FVector Location;
	Location.X = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Y = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Z = 100;
	Transform.SetTranslation(Location);

	// Spawn an initial spawner 
	ASlimeSpawner* SlimeSpawnerActor = GetWorld()->SpawnActor<ASlimeSpawner>(SlimeSpawnerClass, Transform);
}

void AGamesFourGameModeBase::TimeUp()
{
	// Get random location for spawner
	FTransform Transform;
	FVector Location;
	Location.X = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Y = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Z = 100;
	Transform.SetTranslation(Location);

	// Spawn new slime spawner
	ASlimeSpawner* SlimeSpawnerActor = GetWorld()->SpawnActor<ASlimeSpawner>(SlimeSpawnerClass, Transform);
	SlimeSpawnerList.Push(SlimeSpawnerActor);
}

void AGamesFourGameModeBase::ClearSlimeSpawners()
{
	// Delete all slime spawners and their slimes
	for (auto& SlimeSpawner : SlimeSpawnerList)
	{
		SlimeSpawner->ClearSlimes();
		SlimeSpawner->Destroy();
	}
}

// Return slime kills for combo
int AGamesFourGameModeBase::GetSlimeKills()
{
	return SlimeKills;
}

// Return shop gold pool
int AGamesFourGameModeBase::GetShopGold()
{
	return ShopGold;
}
