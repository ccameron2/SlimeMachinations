// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeSpawner.h"

#include "Kismet/GameplayStatics.h"

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
	// Get a list of all slimes
	TArray<AActor*> AllSlimesList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SlimeClass, AllSlimesList);
	
	// Destroy each slime
	for (auto& Slime : AllSlimesList)
	{
		Slime->Destroy();
	}
}

void ASlimeSpawner::TimeUp()
{
	// Get random location for slime
	FTransform Transform;
	FVector Location;
	Location.X = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Y = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Z = FMath::RandRange(100, 200);
	Transform.SetTranslation(Location);

	// Get random scale for slime
	float SlimeScaleOffset = FMath::RandRange(0.2, 0.8);
	Transform.SetScale3D(SlimeScale + FVector(SlimeScaleOffset,SlimeScaleOffset,SlimeScaleOffset));

	// Get a list of all slimes
	TArray<AActor*> AllSlimesList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SlimeClass, AllSlimesList);

	// If slimes are not too plentiful
	if (AllSlimesList.Num() < SlimeLimit)
	{
		// Generate number to decide which slime to spawn
		auto rand = FMath::RandRange(0, 200);
		if (rand >= 199)
		{
			// Spawn boss slime
			Transform.SetLocation(Location + FVector{ 0,0,300 });
			ASlimeEnemy* BossSlimeEnemy = GetWorld()->SpawnActor<ASlimeEnemy>(BossSlimeClass, Transform);
			BossSlimeEnemy->MovementSpeed += AllSlimesList.Num() / 10;
		}
		// Spawn normal slime
		ASlimeEnemy* SlimeEnemy = GetWorld()->SpawnActor<ASlimeEnemy>(SlimeClass, Transform);
		if (SlimeEnemy)
		{
			// Increase movement speed for each slime
			SlimeEnemy->MovementSpeed += AllSlimesList.Num() / 10;
		}
	}

}

