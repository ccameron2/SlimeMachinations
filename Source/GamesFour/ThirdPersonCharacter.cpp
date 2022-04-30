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

	//Create Camera and attach to SpringArm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SparkSpawnpoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spark Spawn Point"));
	SparkSpawnpoint->SetupAttachment(RootComponent);
	SparkSpawnpoint->SetRelativeLocation(FVector{ 30.0f,0.0f,50.0f });

	GameMode = Cast<AGamesFourGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	//Possess player 0
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AThirdPersonCharacter::OnOverlapBegin);

	FTimerHandle ManaTimer;
	GetWorld()->GetTimerManager().SetTimer(ManaTimer, this, &AThirdPersonCharacter::RegenerateMana, ManaRegenTime, true);

	FTimerHandle EnergyTimer;
	GetWorld()->GetTimerManager().SetTimer(EnergyTimer, this, &AThirdPersonCharacter::RegenerateEnergy, EnergyRegenTime, true);

	FTimerHandle GoldTimer;
	GetWorld()->GetTimerManager().SetTimer(GoldTimer, this, &AThirdPersonCharacter::StaticDrainGold, GoldDrainTime, true);
}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Energy < MaxEnergy){ Energy++; }
	if (ExpPoints >= MaxExp) { LevelUp(); }
	if (HealthPoints < 0)
	{	
		if (Lives > 0)
		{
			Lives--;
			Gold = 0;
			MaxHealth = 100;
			MaxMana = 100;
			MaxEnergy = 100;
			MaxExp = 100;
			HealthPoints = MaxHealth;
			Mana = MaxMana;
			Energy = MaxEnergy;
			Level = 1;
			ExpPoints = 0;
			SkillPoints = 0;
			GameMode->ClearSlimeSpawners();
			SetActorLocation(FVector{ 0,300,0 });
		}
		else
		{
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
	//Move character forwards

	if (Sprinting)
	{
		if (Energy > 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = 1000;
			Energy -= 2;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 600;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	if (Controller != nullptr && AxisValue != 0)
	{
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
	if (Mana >= ManaCost)
	{
		FVector Location = SparkSpawnpoint->GetComponentLocation();
		FTransform Transform;
		Transform.SetLocation(Location);
		ASparks* SparkActor = GetWorld()->SpawnActor<ASparks>(MagicClass, Transform);
		SparkActor->SetDamage(Damage);
		Mana -= ManaCost;
	}
}

void AThirdPersonCharacter::Jump()
{
	auto MovementComponent = GetMovementComponent();

	if (Energy > 75 && !MovementComponent->IsFalling())
	{
		Super::Jump();
		Energy -= 75;
	}	
}

void AThirdPersonCharacter::ToggleSprint()
{
	Sprinting = !Sprinting;
}


bool AThirdPersonCharacter::HasSkillPoints()
{
	if (SkillPoints > 0) { return true; }
	else { return false; }
}


void AThirdPersonCharacter::LevelUp()
{
	Level++; 
	ExpPoints = 0.0f; 
	SkillPoints++;
	MaxExp += 10;
}

void AThirdPersonCharacter::BuyManaRegen()
{
	if (Gold >= ManaGoldCost)
	{
		GameMode->ShopGold += ManaGoldCost;
		Gold -= ManaGoldCost;
		ManaRegenAmount++;
		ManaGoldCost += 5;
	}
}

void AThirdPersonCharacter::BuyEnergyRegen()
{
	if (Gold >= EnergyGoldCost)
	{
		GameMode->ShopGold += EnergyGoldCost;
		Gold -= EnergyGoldCost;
		EnergyRegenAmount++;
		EnergyGoldCost += 5;
	}
}

void AThirdPersonCharacter::BuyHealth()
{
	if (Gold >= HealthGoldCost)
	{
		GameMode->ShopGold += HealthGoldCost;
		Gold -= HealthGoldCost;
		HealthPoints = MaxHealth;
		HealthGoldCost += 5;
	}
}

void AThirdPersonCharacter::BuyALife()
{
	if (Gold >= LifeGoldCost)
	{
		GameMode->ShopGold += LifeGoldCost;
		Gold -= LifeGoldCost;
		Lives++;
	}
}

void AThirdPersonCharacter::BuyDamage()
{
	if (Gold >= DamageGoldCost)
	{
		GameMode->ShopGold += 400;
		Gold -= 400;
		Damage += 50;
		DamageGoldCost += 100;
	}
}

void AThirdPersonCharacter::RegenerateMana()
{
	if (Mana < MaxMana)
	{
		Mana += ManaRegenAmount;
	}
}

void AThirdPersonCharacter::RegenerateEnergy()
{
	if (Energy < MaxEnergy)
	{
		Energy += EnergyRegenAmount;
	}
}

void AThirdPersonCharacter::StaticDrainGold()
{
	if (Gold > 0)
	{
		Gold--;
	}	
}

float AThirdPersonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GameMode->SlimeKills = 0;
	HealthPoints -= DamageAmount;
	return 0.0f;
}

void AThirdPersonCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
		AResourcePickup* resourcePickup = Cast<AResourcePickup>(OtherActor);
		if (resourcePickup)
		{
			if (resourcePickup->Type == 0) { if(HealthPoints < MaxHealth ){ HealthPoints++; }}
			if (resourcePickup->Type == 1) { if(Mana		 < MaxMana	 ){ Mana++; }}
			if (resourcePickup->Type == 2) { if(Energy		 < MaxEnergy ){ Energy++; }}
			if (resourcePickup->Type == 3) { if(ExpPoints	 < MaxExp	 ){ ExpPoints++; }}
			if (resourcePickup->Type == 4) { if(Gold		 < MaxGold	 ){ Gold++; }}
			OtherActor->Destroy();
		}
		else
		{
			ASlimeEnemy* slimeEnemy = Cast<ASlimeEnemy>(OtherActor);
			if (slimeEnemy) { UGameplayStatics::ApplyDamage(OtherActor, 10.0f, GetInstigatorController(), this, UDamageType::StaticClass()); }
		}
	}
}
