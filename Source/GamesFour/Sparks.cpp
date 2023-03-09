// Fill out your copyright notice in the Description page of Project Settings.


#include "Sparks.h"
#include "Kismet/Gameplaystatics.h"
#include "EngineUtils.h"

// Sets default values
ASparks::ASparks()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create particle system component and attach to root component
	Sparks = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Sparks Particle Component"));
	SetRootComponent(Sparks);


	// Create collision component for spark to deal damage to slimes
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->InitSphereRadius(12.0f);
}

// Called when the game starts or when spawned
void ASparks::BeginPlay()
{
	Super::BeginPlay();

	// Bind collision dynamic delegates
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASparks::OnOverlapBegin);

	// Start timer for lifespan of projectile
	FTimerHandle LifeTimer;
	GetWorld()->GetTimerManager().SetTimer(LifeTimer, this, &ASparks::TimeUp, Lifetime, false);
}

// Called every frame
void ASparks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Activate effect every tick
	Sparks->Activate();


	// Get reference to all slimes
	TArray<AActor*> Slimes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SlimeClass, Slimes);
	
	// Set min distance very high 
	float MinDistance = 999999;

	// Reference to hold closest slime found so far
	ASlimeEnemy* NearestSlime = nullptr;

	// Loop through all slimes
	for (auto Slime : Slimes)
	{
		// Cast the actor reference to a slime reference
		ASlimeEnemy* SlimeActor = Cast<ASlimeEnemy>(Slime);

		// Calculate distance between slime and projectile
		auto Distance = FVector::Distance(SlimeActor->GetActorLocation(), GetActorLocation());

		// If the distance is the smallest found so far
		if (Distance < MinDistance)
		{
			// Set smallest distance found to current distance
			MinDistance = Distance;
			
			// Assign the current slime to be the current nearest found
			NearestSlime = SlimeActor;
		}
	}

	// If the nearest slime has been found
	if (NearestSlime)
	{
		// Move towards the slime until collision
		auto NewLocation = FMath::VInterpTo(GetActorLocation(), NearestSlime->GetActorLocation(), DeltaTime, 5.0f);
		SetActorLocation(NewLocation);
	}
}

void ASparks::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If the projectile hit a slime
	if (Cast<ASlimeEnemy>(OtherActor))
	{
		// Apply damage to the slime
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		
		// Destroy the projectile
		Destroy();
	}
}

void ASparks::TimeUp()
{
	// Destroy the projectile if lifetime runs outs
	Destroy();
}

void ASparks::SetDamage(float damageAmount)
{
	Damage = damageAmount;
}

