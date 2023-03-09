// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaGenerator.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AManaGenerator::AManaGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create static mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMesh);

	// Create trigger box to activate conversion
	ActivationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Activation Box"));
	ActivationBox->SetupAttachment(RootComponent);

	// Create text to explain conversion rates
	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render"));
	TextRender->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AManaGenerator::BeginPlay()
{
	Super::BeginPlay();

	// Bind dynamic delegates for trigger box
	ActivationBox->OnComponentBeginOverlap.AddDynamic(this, &AManaGenerator::OnOverlapBegin);
	ActivationBox->OnComponentEndOverlap.AddDynamic(this, &AManaGenerator::OnOverlapEnd);

	// Start timer for Mana conversion and pause
	GetWorld()->GetTimerManager().SetTimer(ManaTimer, this, &AManaGenerator::ManaLoop, 0.01, true);
	GetWorldTimerManager().PauseTimer(ManaTimer);

	// Set text invisible intially
	TextRender->SetVisibility(false);
}

// Called every frame
void AManaGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AManaGenerator::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Cast other actor as third person character
	PlayerPawn = Cast<AThirdPersonCharacter>(OtherActor);

	// If successful
	if (Cast<AThirdPersonCharacter>(OtherActor))
	{
		// Render the explaination text
		TextRender->SetVisibility(true);
	}

	// Start mana conversion
	GetWorldTimerManager().UnPauseTimer(ManaTimer);
}

void AManaGenerator::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Stop mana conversion
	GetWorldTimerManager().PauseTimer(ManaTimer);

	// If the character left the generator
	if (Cast<AThirdPersonCharacter>(OtherActor))
	{
		// Set text to be invisible
		TextRender->SetVisibility(false);
	}
}

void AManaGenerator::ManaLoop()
{
	if (PlayerPawn)
	{
		// If the player has enough energy
		if (PlayerPawn->Energy > EnergyCost)
		{
			// If their mana isnt full
			if (PlayerPawn->Mana  < PlayerPawn->MaxMana)
			{
				// If they have enough gold
				if (PlayerPawn->Gold > 0)
				{
					// Convert energy and gold to mana
					PlayerPawn->Energy -= EnergyCost;
					PlayerPawn->Gold--;
					PlayerPawn->Mana+= 2;
				}
			}
		}
	}
}


