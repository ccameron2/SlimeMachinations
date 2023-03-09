// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h" 
#include "ThirdPersonCharacter.h"
#include "Components/TextRenderComponent.h" 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManaGenerator.generated.h"

UCLASS()
class GAMESFOUR_API AManaGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManaGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// Static mesh for generator
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	// Trigger box for activation
	UPROPERTY(EditAnywhere)
		UBoxComponent* ActivationBox;

	// Energy cost to convert
	UPROPERTY(EditAnywhere)
		int EnergyCost = 3;

	// Dynamic Delegates for Trigger box
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Loop to convert mana
	UFUNCTION()
		void ManaLoop();

	// Timer for mana conversion
	UPROPERTY()
		FTimerHandle ManaTimer;

	// Pawn to recognise the player
	UPROPERTY()
		AThirdPersonCharacter* PlayerPawn;

	// Text to explain conversion rate
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* TextRender;

};
