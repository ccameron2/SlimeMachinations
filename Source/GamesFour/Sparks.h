// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Particles/ParticleSystemComponent.h" 
#include "SlimeEnemy.h"
#include "Components/SphereComponent.h" 
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
	
	// Set damage of projectile
	UFUNCTION()
		void SetDamage(float damageAmount);

private:

	// Particles for projectile
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* Sparks;

	// Slime class to target
	UPROPERTY(EditAnywhere)
		TSubclassOf<ASlimeEnemy> SlimeClass;

	// Collision component to apply damage to slimes
	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollision;

	// Damage of projectile
	UPROPERTY(EditAnywhere)
		float Damage = 50.0f;

	// Lifetime of projectile
	UPROPERTY(EditAnywhere)
		float Lifetime = 1.0f;

	// Dynamic delegate for collision
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	// Function to call when timer has elapsed
	UFUNCTION()
		void TimeUp();

};
