// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeSpawner.h"

// Sets default values
ASlimeSpawner::ASlimeSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASlimeSpawner::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle SpawnTimer;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ASlimeSpawner::TimeUp, SpawnDelay, true);
}

// Called every frame
void ASlimeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlimeSpawner::ClearSlimes()
{
	for (auto& Slime : SlimeList)
	{
		Slime->Destroy();
	}
}

void ASlimeSpawner::TimeUp()
{
	FTransform Transform;
	FVector Location;
	Location.X = FMath::RandRange(0, 500);
	Location.Y = FMath::RandRange(0, 500);
	Location.Z = FMath::RandRange(0, 200);
	Transform.SetScale3D(SlimeScale);
	Transform.SetTranslation(Location);
	ASlimeEnemy* SlimeEnemy = GetWorld()->SpawnActorDeferred<ASlimeEnemy>(SlimeClass, Transform);
    //SlimeEnemy->Type = FMath::RandRange(0, SlimeEnemy->MaterialList.Num() - 1);
    SlimeEnemy->FinishSpawning(Transform);
	SlimeList.Push(SlimeEnemy);
}

