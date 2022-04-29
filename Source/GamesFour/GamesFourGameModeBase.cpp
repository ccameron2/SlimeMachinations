// Copyright Epic Games, Inc. All Rights Reserved.


#include "GamesFourGameModeBase.h"

void AGamesFourGameModeBase::StartPlay()
{
	Super::StartPlay();

	FTransform Transform;
	FTimerHandle SpawnerTimer;
	GetWorld()->GetTimerManager().SetTimer(SpawnerTimer, this, &AGamesFourGameModeBase::TimeUp, SlimeSpawnerCooldown, true);
	
	FVector Location;
	Location.X = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Y = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Z = 100;

	Transform.SetTranslation(Location);
	ASlimeSpawner* SlimeSpawnerActor = GetWorld()->SpawnActor<ASlimeSpawner>(SlimeSpawnerClass, Transform);
}

void AGamesFourGameModeBase::TimeUp()
{
	FTransform Transform;

	FVector Location;
	Location.X = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Y = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Z = 100;

	Transform.SetTranslation(Location);

	ASlimeSpawner* SlimeSpawnerActor = GetWorld()->SpawnActor<ASlimeSpawner>(SlimeSpawnerClass, Transform);
	SlimeSpawnerList.Push(SlimeSpawnerActor);
}

void AGamesFourGameModeBase::ClearSlimeSpawners()
{
	for (auto& SlimeSpawner : SlimeSpawnerList)
	{
		SlimeSpawner->ClearSlimes();
		SlimeSpawner->Destroy();
	}
}

int AGamesFourGameModeBase::GetSlimeKills()
{
	return SlimeKills;
}

int AGamesFourGameModeBase::GetShopGold()
{
	return ShopGold;
}
