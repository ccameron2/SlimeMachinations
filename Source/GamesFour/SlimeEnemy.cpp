// Fill out your copyright notice in the Description page of Project Settings.

#include "SlimeEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASlimeEnemy::ASlimeEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SlimeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Slime Mesh"));
	SetRootComponent(SlimeMesh);
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called when the game starts or when spawned
void ASlimeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlimeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BounceTo(PlayerPawn);
}

// Called to bind functionality to input
void ASlimeEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASlimeEnemy::AddImpulse(FVector Direction)
{
	FVector Location = GetActorLocation();
	if (GetVelocity().X < MaxSpeed && GetVelocity().Y < MaxSpeed && GetVelocity().Z < MaxSpeed)
	{
		if (Location.Z < GroundLevel)
		{
			SlimeMesh->AddImpulse(FVector(0, 0, JumpStrength));
		}
		else
		{
			SlimeMesh->AddImpulse((Direction) * (MovementSpeed / 1000));
		}
	}	
}

float ASlimeEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Take Damage"));
	HealthPoints -= DamageAmount;
	return 0.0f;
}

void ASlimeEnemy::BounceTo(AActor* Target)
{
	FVector PlayerLocation = Target->GetActorLocation();
	FVector Location = GetActorLocation();
	FVector Direction = PlayerLocation - Location;
	AddImpulse(Direction);
}

