// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaGenerator.h"

// Sets default values
AManaGenerator::AManaGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMesh);

	ActivationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Activation Box"));
	ActivationBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AManaGenerator::BeginPlay()
{
	Super::BeginPlay();
	ActivationBox->OnComponentBeginOverlap.AddDynamic(this, &AManaGenerator::OnOverlapBegin);
	ActivationBox->OnComponentEndOverlap.AddDynamic(this, &AManaGenerator::OnOverlapEnd);
	GetWorld()->GetTimerManager().SetTimer(ManaTimer, this, &AManaGenerator::ManaLoop, 0.01, true);
	GetWorldTimerManager().PauseTimer(ManaTimer);

}

// Called every frame
void AManaGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AManaGenerator::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerPawn = Cast<AThirdPersonCharacter>(OtherActor);
	GetWorldTimerManager().UnPauseTimer(ManaTimer);
}

void AManaGenerator::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().PauseTimer(ManaTimer);
}

void AManaGenerator::ManaLoop()
{
	if (PlayerPawn)
	{
		if (PlayerPawn->Energy > EnergyCost)
		{
			PlayerPawn->Energy -= EnergyCost;
			if (PlayerPawn->Mana  < PlayerPawn->MaxMana)
			{
				PlayerPawn->Mana++;
			}
		}
	}
}


