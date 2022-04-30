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

	UFUNCTION()
		void ClearSlimes();

	UPROPERTY(EditAnywhere)
		int SlimeLimit = 2000;

private:

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASlimeEnemy> SlimeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASlimeEnemy> BossSlimeClass;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
		float SpawnDelay = 5.0f;

	UPROPERTY(EditAnywhere)
		FVector SlimeScale = FVector(1.0f,1.0f,1.0f);

	UPROPERTY(EditAnywhere)
		float SpawnRange = 500;

	UFUNCTION()
		void TimeUp();

};
