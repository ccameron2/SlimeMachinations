// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlimeEnemy.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlimeSpawner.generated.h"

UCLASS()
class GAMESFOUR_API ASlimeSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlimeSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Destroy slime actors
	UFUNCTION()
		void ClearSlimes();

	// Limit for slimes in world
	UPROPERTY(EditAnywhere)
		int SlimeLimit = 2000;

private:

	// Class of slime to spawn
	UPROPERTY(EditAnywhere)
		TSubclassOf<ASlimeEnemy> SlimeClass;

	// Class of boss slime to spawn
	UPROPERTY(EditAnywhere)
		TSubclassOf<ASlimeEnemy> BossSlimeClass;

	// Collision component for dealing damage to the player on overlap
	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollision;

	// Delay between spawning slimes
	UPROPERTY(EditAnywhere)
		float SpawnDelay = 5.0f;

	// Initial scale of the slimes
	UPROPERTY(EditAnywhere)
		FVector SlimeScale = FVector(1.0f,1.0f,1.0f);

	// Range that the slimes can spawn
	UPROPERTY(EditAnywhere)
		float SpawnRange = 500;

	// Function to call when timer has elapsed
	UFUNCTION()
		void TimeUp();

};
