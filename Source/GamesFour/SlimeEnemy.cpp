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
	SlimeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Slime Mesh"));
	SetRootComponent(SlimeMesh);
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	SlimeMesh->SetEnableGravity(true);
	SlimeMesh->SetSimulatePhysics(true);
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->InitSphereRadius(17.0f);
}

// Called when the game starts or when spawned
void ASlimeEnemy::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle MovementTimer;
	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this, &ASlimeEnemy::MoveTimerUp, BounceDelay, true);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASlimeEnemy::OnOverlapBegin);
	SlimeMesh->SetMaterial(0, SlimeMats[FMath::RandRange(0,SlimeMats.Num()-1)]);
}

// Called every frame
void ASlimeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HealthPoints <= 0)
	{
		AGamesFourGameModeBase* GameMode = Cast<AGamesFourGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		auto Drops = DropAmount + (GameMode->SlimeKills / 2);
		for (int i = 0; i < Drops; i++)
		{
			FVector Location = GetActorLocation();
			FTransform Transform;
			Transform.SetScale3D(FVector{ 0.08,0.08,0.08 });
			Transform.SetLocation(Location + FVector{ float(FMath::RandRange(-DropSpread,DropSpread)),float(FMath::RandRange(-DropSpread,DropSpread)),float(FMath::RandRange(-DropSpread,DropSpread)) });
			AResourcePickup* ResourcePickup = GetWorld()->SpawnActorDeferred<AResourcePickup>(ResourcePickupClass, Transform);
			ResourcePickup->Type = FMath::RandRange(0, ResourcePickup->MaterialList.Num() - 1);
			ResourcePickup->FinishSpawning(Transform);
		}
		GameMode->SlimeKills++;
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
	FVector Location = GetActorLocation();
	if (GetVelocity().X < MaxSpeed && GetVelocity().Y < MaxSpeed && GetVelocity().Z < MaxSpeed)
	{
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SlimeDamage"));
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

