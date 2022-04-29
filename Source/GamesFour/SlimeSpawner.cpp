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
	TArray<AActor*> AllSlimesList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SlimeClass, AllSlimesList);
	for (auto& Slime : AllSlimesList)
	{
		Slime->Destroy();
	}
}

void ASlimeSpawner::TimeUp()
{
	FTransform Transform;
	FVector Location;
	Location.X = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Y = FMath::RandRange(-SpawnRange, SpawnRange);
	Location.Z = FMath::RandRange(0, 200);
	float SlimeScaleOffset = FMath::RandRange(-0.5, 0.5);
	Transform.SetScale3D(SlimeScale + FVector(SlimeScaleOffset,SlimeScaleOffset,SlimeScaleOffset));
	Transform.SetTranslation(Location);
	TArray<AActor*> AllSlimesList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SlimeClass, AllSlimesList);
	if (AllSlimesList.Num() < SlimeLimit)
	{
		ASlimeEnemy* SlimeEnemy = GetWorld()->SpawnActor<ASlimeEnemy>(SlimeClass, Transform);
	}

}

