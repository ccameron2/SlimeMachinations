// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h" 
#include "Materials/MaterialInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourcePickup.generated.h"

UCLASS()
class GAMESFOUR_API AResourcePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourcePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Type of resource
	UPROPERTY(EditAnywhere)
		int Type = 0;

	// List of materials
	UPROPERTY(EditAnywhere)
		TArray<UMaterialInterface*> MaterialList;

private:

	// Mesh for resource
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ResourceMesh;

	// Intensity of magnet pull
	UPROPERTY(EditAnywhere)
		float MagnetIntensity = 0.9f;

	// Range of magnet activation
	UPROPERTY(EditAnywhere)
		float MagnetRange = 500.0f;

	// Lifetime timer
	UPROPERTY()
		FTimerHandle ResourceTimer;

	// Function to call when lifetime is up
	UFUNCTION()
		void TimeUp();

	// Lifetime of the resource
	UPROPERTY(EditAnywhere)
		float Lifetime = 2.0f;

};
