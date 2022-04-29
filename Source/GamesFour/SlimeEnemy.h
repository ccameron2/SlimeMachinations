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

	void AddImpulse(FVector Direction);

	UPROPERTY(EditAnywhere)
		float MovementSpeed = 80;

	UPROPERTY(EditAnywhere)
		float JumpStrength = 150;

	UPROPERTY(EditAnywhere)
		float GroundLevel = 36;

	UPROPERTY(EditAnywhere)
		float MaxSpeed = 300;

	UPROPERTY(EditAnywhere)
		float HealthPoints = 100;

	UPROPERTY(EditAnywhere)
		int DropAmount = 25;

	UPROPERTY(EditAnywhere)
		int DropSpread = 50;

	UPROPERTY(EditAnywhere)
		int Damage = 20;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollision;

	UFUNCTION()
		void BounceTo(AActor* Target);

	UFUNCTION()
		void MoveTimerUp();

	UPROPERTY(EditAnywhere)
		float BounceDelay = 0.5f;

	APawn* PlayerPawn;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SlimeMesh;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AResourcePickup> ResourcePickupClass;
	UPROPERTY(EditAnywhere)
		TArray<UMaterialInterface*> SlimeMats;
};
