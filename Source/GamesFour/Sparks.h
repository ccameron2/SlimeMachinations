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
	
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* Sparks;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASlimeEnemy> SlimeClass;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
		float Damage = 50.0f;

	UPROPERTY(EditAnywhere)
		float Lifetime = 1.0f;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
		void TimeUp();

	UFUNCTION()
		void SetDamage(float damageAmount);

};
