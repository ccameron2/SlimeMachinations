// Fill out your copyright notice in the Description page of Project Settings.


#include "Sparks.h"

// Sets default values
ASparks::ASparks()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create particle system component and attach to root component
	Sparks = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Sparks Particle Component"));
	Sparks->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASparks::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASparks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Activate effect every tick
	Sparks->Activate();
	auto newLoc = FMath::VInterpTo(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), DeltaTime, 5.0f);
	SetActorLocation(newLoc);
}

