// Fill out your copyright notice in the Description page of Project Settings.

#include "SlimeEnemy.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "ThirdPersonCharacter.h"
#include "GamesFourGameModeBase.h"

// Sets default values
ASlimeEnemy::ASlimeEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create static mesh for slime
	SlimeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Slime Mesh"));
	SetRootComponent(SlimeMesh);

	// Enable gravity and physics
	SlimeMesh->SetEnableGravity(true);
	SlimeMesh->SetSimulatePhysics(true);

	// Create collision component for slime
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->InitSphereRadius(18.0f);

}

// Called when the game starts or when spawned
void ASlimeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Create timer to delay movement
	FTimerHandle MovementTimer;
	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this, &ASlimeEnemy::MoveTimerUp, BounceDelay, true);

	// Assign dynamic delegates
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASlimeEnemy::OnOverlapBegin);

	// Assign random material from list
	SlimeMesh->SetMaterial(0, SlimeMats[FMath::RandRange(0,SlimeMats.Num()-1)]);

	// Get reference to player pawn from world
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

}

// Called every frame
void ASlimeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If slime is dead
	if (HealthPoints <= 0)
	{
		// Get reference to game mode
		AGamesFourGameModeBase* GameMode = Cast<AGamesFourGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		// Amount of drops to spawn when killed
		// Modified by slime combo kills
		auto Drops = DropAmount + (GameMode->SlimeKills / 2);

		// Spawn resources
		for (int i = 0; i < Drops; i++)
		{
			// Set a random location for the drop
			FVector Location = GetActorLocation();
			FTransform Transform;
			Transform.SetScale3D(FVector{ 0.08,0.08,0.08 });
			Transform.SetLocation(Location + FVector{ float(FMath::RandRange(-DropSpread,DropSpread)),float(FMath::RandRange(-DropSpread,DropSpread)),float(FMath::RandRange(-DropSpread,DropSpread)) });

			// Spawn resource drop
			AResourcePickup* ResourcePickup = GetWorld()->SpawnActorDeferred<AResourcePickup>(ResourcePickupClass, Transform);
			ResourcePickup->Type = FMath::RandRange(0, ResourcePickup->MaterialList.Num() - 1);
			ResourcePickup->FinishSpawning(Transform);
		}

		// Increment slime kills combo
		GameMode->SlimeKills++;

		// Destroy slime
		Destroy();
	}
}

// Called to bind functionality to input
void ASlimeEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASlimeEnemy::AddImpulse(FVector Direction)
{
	// Get actor location
	FVector Location = GetActorLocation();

	// Check for max speed
	if (GetVelocity().X < MaxSpeed && GetVelocity().Y < MaxSpeed && GetVelocity().Z < MaxSpeed)
	{
		// 
		if (Location.Z < GroundLevel)
		{
			Direction.Normalize();
			SlimeMesh->AddImpulse(FVector(0, 0, JumpStrength));
			SlimeMesh->AddImpulse(Direction * MovementSpeed);
		}
	
	}	
}


void ASlimeEnemy::BounceTo(AActor* Target)
{
	FVector PlayerLocation = Target->GetActorLocation();
	FVector Location = GetActorLocation();
	FVector Direction = PlayerLocation - Location;
	AddImpulse(Direction);
}

void ASlimeEnemy::MoveTimerUp()
{
	if (PlayerPawn)
	{
		BounceTo(PlayerPawn);
	}
}

float ASlimeEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HealthPoints -= DamageAmount;
	return 0.0f;
}

void ASlimeEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AThirdPersonCharacter>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		Destroy();
	}
}

