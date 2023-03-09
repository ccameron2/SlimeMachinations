// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ResourcePickup.h"
#include "Components/SphereComponent.h" 

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SlimeEnemy.generated.h"

UCLASS()
class GAMESFOUR_API ASlimeEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASlimeEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Use physics to move the slime
	void AddImpulse(FVector Direction);

	// Speed the slime bounces
	UPROPERTY(EditAnywhere)
		float MovementSpeed = 80;

	// Strength of upwards impulse
	UPROPERTY(EditAnywhere)
		float JumpStrength = 100;

	// Minimum Z level where a slime can jump
	UPROPERTY(EditAnywhere)
		float GroundLevel = 40;

	// Maximum speed
	UPROPERTY(EditAnywhere)
		float MaxSpeed = 200;

	// Health of slime
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float HealthPoints = FMath::RandRange(50,150);

	// Max health of slime
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth = HealthPoints;

	// Amount of resource drops
	UPROPERTY(EditAnywhere)
		int DropAmount = 25;

	// Spread of resources in both directions when dropped
	UPROPERTY(EditAnywhere)
		int DropSpread = 50;

	// Damage slime inflicts on player when in contact
	UPROPERTY(EditAnywhere)
		int Damage = 20;

	// Collision component for damaging player
	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollision;

	// Custom movement function for slimes
	UFUNCTION()
		void BounceTo(AActor* Target);

	// Function to call when timer has elapsed
	UFUNCTION()
		void MoveTimerUp();

	// Length of the move timer
	UPROPERTY(EditAnywhere)
		float BounceDelay = 0.5f;

	// Player Pawn reference
	APawn* PlayerPawn;

	// Overrided damage function
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	// Collision component
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	// Static Mesh for slime
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SlimeMesh;
	
	// Class of Resources to drop
	UPROPERTY(EditAnywhere)
		TSubclassOf<AResourcePickup> ResourcePickupClass;
	
	// Materials to choose from when spawning a slime
	UPROPERTY(EditAnywhere)
		TArray<UMaterialInterface*> SlimeMats;
};
