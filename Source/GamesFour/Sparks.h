// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Particles/ParticleSystemComponent.h" 
#include "SlimeEnemy.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sparks.generated.h"

UCLASS()
class GAMESFOUR_API ASparks : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASparks();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* Sparks;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASlimeEnemy> SlimeClass;
};
