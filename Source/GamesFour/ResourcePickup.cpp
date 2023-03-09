// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourcePickup.h"
#include "Kismet/KismetMathLibrary.h" 

// Sets default values
AResourcePickup::AResourcePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh for resource
	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Health Mesh"));
	ResourceMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AResourcePickup::BeginPlay()
{
	Super::BeginPlay();
	
	// Start a timer for lifetime
	GetWorld()->GetTimerManager().SetTimer(ResourceTimer, this, &AResourcePickup::TimeUp, Lifetime, false);

	// Set random material from list
	ResourceMesh->SetMaterial(0, MaterialList[Type]);
}

// Called every frame
void AResourcePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Get location and player reference
	FVector Location = GetActorLocation();
	AActor* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (PlayerPawn)
	{
		// Get player location and calculate distance to travel
		FVector PlayerLocation = PlayerPawn->GetActorLocation() + FVector{ 0,0,200 };
		float DistanceToTravel = FVector::Distance(PlayerLocation, Location);

		// If distance is less than attraction range
		if (DistanceToTravel < MagnetRange)
		{
			// Add impulse towards player
			FVector UnitDirectionVector = UKismetMathLibrary::GetDirectionUnitVector(Location, PlayerLocation);
			ResourceMesh->AddImpulse(UnitDirectionVector * ((MagnetIntensity * DistanceToTravel) / 40));
		}
		// If lifetime is up
		if (!ResourceTimer.IsValid())
		{
			// Move directly to player until reached
			auto newLoc = FMath::VInterpTo(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() + FVector { 0, 0, 100 }, DeltaTime, 8.0f);
			SetActorLocation(newLoc);
		}
	}

}

void AResourcePickup::TimeUp()
{
	ResourceTimer.Invalidate();
}



