// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
		float MovementSpeed = 100;

	UPROPERTY(EditAnywhere)
		float JumpStrength = 150;

	UPROPERTY(EditAnywhere)
		float GroundLevel = 16;

	UPROPERTY(EditAnywhere)
		float MaxSpeed = 150;

	UPROPERTY(EditAnywhere)
		float HealthPoints = 50;

	UFUNCTION()
		void BounceTo(AActor* Target);

	APawn* PlayerPawn;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	
private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SlimeMesh;

};
