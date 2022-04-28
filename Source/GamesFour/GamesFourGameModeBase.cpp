// Copyright Epic Games, Inc. All Rights Reserved.


#include "GamesFourGameModeBase.h"

void AGamesFourGameModeBase::StartPlay()
{
	Super::StartPlay();

	
	FTimerHandle LifeTimer;
	GetWorld()->GetTimerManager().SetTimer(LifeTimer, this, &AGamesFourGameModeBase::TimeUp, SlimeSpawnerCooldown, true);
		
	//ASlimeEnemy* SlimeActor = GetWorld()->SpawnActor<ASlimeEnemy>(SlimeClass, Transform);

	//AResourcePickup* ResourcePickup = GetWorld()->SpawnActorDeferred<AResourcePickup>(ResourceClass, Transform);
	//ResourcePickup->Type = FMath::RandRange(0, ResourcePickup->MaterialList.Num() - 1);
	//ResourcePickup->FinishSpawning(Transform);
}

void AGamesFourGameModeBase::TimeUp()
{
	FActorSpawnParameters SpawnParams;
	FTransform Transform;

	FVector Location;
	Location.X = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Y = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Z = FMath::RandRange(0, 100);

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
