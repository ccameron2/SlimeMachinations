// Fill out your copyright notice in the Description page of Project Settings.


#include "Sparks.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

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

	TArray<AActor*> Slimes;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SlimeClass, Slimes);
	
	float MinDistance = 999999;
	ASlimeEnemy* NearestSlime = nullptr;

	for (auto Slime : Slimes)
	{
		ASlimeEnemy* SlimeActor = Cast<ASlimeEnemy>(Slime);
		auto Distance = FVector::Distance(SlimeActor->GetActorLocation(), GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestSlime = SlimeActor;
		}
	}

	if (NearestSlime)
	{
		auto NewLocation = FMath::VInterpTo(GetActorLocation(), NearestSlime->GetActorLocation(), DeltaTime, 5.0f);
		SetActorLocation(NewLocation);
	}
}

