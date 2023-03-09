// Fill out your copyright notice in the Description page of Project Settings.
#include "ThirdPersonCharacter.h"
#include "ResourcePickup.h"
#include "SlimeEnemy.h"
#include "Kismet/Gameplaystatics.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "GameFramework/PawnMovementComponent.h" 

// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create spring arm and attach to the root component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));

	// Spring Arm settings
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->SetRelativeRotation((new FRotator(0.0f, 0.0f, 0.0f))->Quaternion());
	SpringArm->SetRelativeLocation(FVector(-361.0f, -305.0f, 113.0f));
	SpringArm->SetupAttachment(RootComponent);

	// Create Camera and attach to SpringArm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Point to spawn spikes (to prevent sparks from coming from the rear)
	SparkSpawnpoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spark Spawn Point"));
	SparkSpawnpoint->SetupAttachment(RootComponent);
	SparkSpawnpoint->SetRelativeLocation(FVector{ 30.0f,0.0f,50.0f });

	// Get reference to game mode
	GameMode = Cast<AGamesFourGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	//Possess player 0
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add dynamic delegate for collision
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AThirdPersonCharacter::OnOverlapBegin);

	// Setup timer for mana regen
	FTimerHandle ManaTimer;
	GetWorld()->GetTimerManager().SetTimer(ManaTimer, this, &AThirdPersonCharacter::RegenerateMana, ManaRegenTime, true);

	// Setup timer for energy regen
	FTimerHandle EnergyTimer;
	GetWorld()->GetTimerManager().SetTimer(EnergyTimer, this, &AThirdPersonCharacter::RegenerateEnergy, EnergyRegenTime, true);

	// Setup timer for gold drain
	FTimerHandle GoldTimer;
	GetWorld()->GetTimerManager().SetTimer(GoldTimer, this, &AThirdPersonCharacter::StaticDrainGold, GoldDrainTime, true);
}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Add baseline energy regen
	if(Energy < MaxEnergy){ Energy++; }

	// Check for a level up
	if (ExpPoints >= MaxExp) { LevelUp(); }
	
	// If player dies
	if (HealthPoints < 0)
	{	
		// If they have a life left
		if (Lives > 0)
		{
			// Deduct a life
			Lives--;

			// Set max values back to 0
			MaxHealth = 100;
			MaxMana = 100;
			MaxEnergy = 100;
			MaxExp = 100;

			// Set stats back to starting point
			Gold = 0;
			HealthPoints = MaxHealth;
			Mana = MaxMana;
			Energy = MaxEnergy;
			Level = 1;
			ExpPoints = 0;
			SkillPoints = 0;

			// Clear all slime spawners
			GameMode->ClearSlimeSpawners();

			// Set location back to centre of level
			SetActorLocation(FVector{ 0,300,0 });
		}
		else
		{
			// Destroy the character
			Destroy();
		}
	}
}

// Called to bind functionality to input
void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AThirdPersonCharacter::MoveForward(float AxisValue)
{
	// Move character forwards
	// If sprinting has been toggled
	if (Sprinting)
	{
		// If player has energy
		if (Energy > 0)
		{
			// Sprint and deduct energy
			GetCharacterMovement()->MaxWalkSpeed = 1000;
			Energy -= 2;
		}
		else
		{
			Sprinting = false;
		}
	}
	else
	{
		// Move at normal speed
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}

	// If there is input
	if (Controller != nullptr && AxisValue != 0)
	{
		// Get input from controller and add movement in desired direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void AThirdPersonCharacter::Strafe(float AxisValue)
{
	//Strafe character left and right
	if (Controller != nullptr && AxisValue != 0)
	{
		// Get input from controller and add movement in desired direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
	//AddMovementInput(GetActorRightVector() * AxisValue);
}

void AThirdPersonCharacter::Turn(float AxisValue)
{
	//Turn character left and right
	AddControllerYawInput(AxisValue);
}

void AThirdPersonCharacter::LookUp(float AxisValue)
{
	//Rotate view up and down
	AddControllerPitchInput(AxisValue);
}

void AThirdPersonCharacter::Fire()
{
	// If the player has enough mana
	if (Mana >= ManaCost)
	{
		// Get a location for the spawn point of the projectile
		FVector Location = SparkSpawnpoint->GetComponentLocation();
		FTransform Transform;
		Transform.SetLocation(Location);

		// Spawn the projectile and set its damage
		ASparks* SparkActor = GetWorld()->SpawnActor<ASparks>(MagicClass, Transform);
		SparkActor->SetDamage(Damage);

		// Deduct mana from the player
		Mana -= ManaCost;
	}
}

void AThirdPersonCharacter::Jump()
{
	// Get reference to movement component
	auto MovementComponent = GetMovementComponent();

	// If player has enough energy and is on the ground
	if (Energy > 75 && !MovementComponent->IsFalling())
	{
		// Call the parent jump function
		Super::Jump();

		// Deduct energy from the player
		Energy -= 75;
	}	
}

void AThirdPersonCharacter::ToggleSprint()
{
	// Toggle sprinting
	Sprinting = !Sprinting;
}


bool AThirdPersonCharacter::HasSkillPoints()
{
	// Check if the player has skill points available
	if (SkillPoints > 0) { return true; }
	else { return false; }
}


void AThirdPersonCharacter::LevelUp()
{
	// Increase level
	Level++; 

	// Reset exp
	ExpPoints = 0.0f;

	// Award a skill point
	SkillPoints++;

	// Increase exp threshold to level up
	MaxExp += 10;
}

void AThirdPersonCharacter::BuyManaRegen()
{
	// If player has enough gold
	if (Gold >= ManaGoldCost)
	{
		// Trade gold with the shop
		GameMode->ShopGold += ManaGoldCost;
		Gold -= ManaGoldCost;

		// Increase mana regeneration amount
		ManaRegenAmount += 5;

		// Increase the cost of the purchase
		ManaGoldCost += 5;
	}
}

void AThirdPersonCharacter::BuyEnergyRegen()
{
	// If player has enough gold
	if (Gold >= EnergyGoldCost)
	{
		// Trade gold with the shop
		GameMode->ShopGold += EnergyGoldCost;
		Gold -= EnergyGoldCost;

		// Increase energy regeneration amount
		EnergyRegenAmount++;

		// Increase the cost of the purchase
		EnergyGoldCost += 5;
	}
}

void AThirdPersonCharacter::BuyHealth()
{
	// If player has enough gold
	if (Gold >= HealthGoldCost)
	{
		// Trade gold with the shop
		GameMode->ShopGold += HealthGoldCost;
		Gold -= HealthGoldCost;

		// Restore health to max
		HealthPoints = MaxHealth;

		// Increase the cost of the purchase
		HealthGoldCost += 5;
	}
}

void AThirdPersonCharacter::BuyALife()
{
	// If player has enough gold
	if (Gold >= LifeGoldCost)
	{
		// Trade gold with the shop
		GameMode->ShopGold += LifeGoldCost;
		Gold -= LifeGoldCost;

		// Give the player an extra life
		Lives++;
	}
}

void AThirdPersonCharacter::BuyDamage()
{
	// If player has enough gold
	if (Gold >= DamageGoldCost)
	{
		// Trade gold with the shop
		GameMode->ShopGold += DamageGoldCost;
		Gold -= DamageGoldCost;

		// Increase player damage
		Damage += 50;

		// Increase cost of purchase
		DamageGoldCost += 100;
	}
}

void AThirdPersonCharacter::RegenerateMana()
{
	// If player is not at max mana
	if (Mana < MaxMana)
	{
		// Give mana to player 
		Mana += ManaRegenAmount;
	}
}

void AThirdPersonCharacter::RegenerateEnergy()
{
	// If player is not at max energy
	if (Energy < MaxEnergy)
	{
		// Give energy to player 
		Energy += EnergyRegenAmount;
	}
}

void AThirdPersonCharacter::StaticDrainGold()
{
	// If the player has gold
	if (Gold > 0)
	{
		// Take gold from them
		Gold--;
	}	
}

float AThirdPersonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Reset slime kills combo
	GameMode->SlimeKills = 0;

	// Reduce player health points by damage amount
	HealthPoints -= DamageAmount;

	return 0.0f;
}

void AThirdPersonCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Get reference to resource pickup from delegate
		AResourcePickup* resourcePickup = Cast<AResourcePickup>(OtherActor);

		// Determine the type of resource and increment correct resource value on player
		if (resourcePickup)
		{
			if (resourcePickup->Type == 0) { if(HealthPoints < MaxHealth ){ HealthPoints++; }}	// Health
			if (resourcePickup->Type == 1) { if(Mana		 < MaxMana	 ){ Mana++; }}			// Mana
			if (resourcePickup->Type == 2) { if(Energy		 < MaxEnergy ){ Energy++; }}		// Energy
			if (resourcePickup->Type == 3) { if(ExpPoints	 < MaxExp	 ){ ExpPoints++; }}		// Experience
			if (resourcePickup->Type == 4) { if(Gold		 < MaxGold	 ){ Gold++; }}			// Gold

			// Destroy the resource once collected
			OtherActor->Destroy();
		}
		else
		{
			// Apply damage to the slime when they hit the player
			ASlimeEnemy* slimeEnemy = Cast<ASlimeEnemy>(OtherActor);
			if (slimeEnemy) { UGameplayStatics::ApplyDamage(OtherActor, 10.0f, GetInstigatorController(), this, UDamageType::StaticClass()); }
		}
	}
}
