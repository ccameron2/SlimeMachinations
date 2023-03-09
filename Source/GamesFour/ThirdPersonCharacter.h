// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Sparks.h"
#include "GamesFourGameModeBase.h"
#include "Components/SceneComponent.h" 

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

UCLASS()
class GAMESFOUR_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Control functions
	void MoveForward(float AxisValue);
	void Strafe(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);
	void Fire();
	void Jump();
	void ToggleSprint();

	// Sprint toggle 
	bool Sprinting = false;

	// Movement speed of the player
	UPROPERTY(EditAnywhere, Category = "Stats")
		float MoveSpeed = 200.0f;

	// Rotation speed of the player
	UPROPERTY(EditAnywhere, Category = "Stats")
		float RotationSpeed = 500.0f;

	// Max health points that can be held
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxHealth = 100.0f;

	// Max energy points that can be held
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxEnergy = 100.0f;

	// Max experience before a level up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxExp = 100.0f;
	
	// Max mana that can be held
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxMana = 100.0f;

	// Max gold that can be held
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxGold = 999.0f;

	// Amount to upgrade a stat with a skill point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float UpgradeAmount = 20.0f;

	// Level up the player
	UFUNCTION(BlueprintCallable)
		void LevelUp();

	// Returns true if the player has available skill points
	UFUNCTION(BlueprintCallable)
		bool HasSkillPoints();

	// Current health of the player 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
		float HealthPoints = MaxHealth;

	// Current experience points collected
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
		float ExpPoints = 0.0f;

	// Current energy of the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
		float Energy = MaxEnergy;

	// Current amount of gold collected
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
		float Gold = 0.0f;

	// Current mana of the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
		float Mana = MaxMana;

	// Current extra lives available
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
		int Lives = 1;

	// Current skill points available
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
		int SkillPoints = 0;

	// Time to wait after every gold drain
	UPROPERTY(EditAnywhere, Category = "Resources")
		float GoldDrainTime = 3.0f;

	// Current player level
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
		int Level = 1;

	// Mana cost to fire projectile
	UPROPERTY(EditAnywhere, Category = "Stats")
		float ManaCost = 10.0f;

	// Time to wait before regenerating mana
	UPROPERTY(EditAnywhere, Category = "Stats")
		float ManaRegenTime = 2.0f;

	// Amount of mana to regenerate
	UPROPERTY(EditAnywhere, Category = "Stats")
		float ManaRegenAmount = 10.0f;

	// Time to wait before regenerating energy
	UPROPERTY(EditAnywhere, Category = "Stats")
		float EnergyRegenTime = 1.0f;

	// Amount of energy to regenerate
	UPROPERTY(EditAnywhere, Category = "Stats")
		float EnergyRegenAmount = 2.0f;

	// Current damage of the player
	UPROPERTY(EditAnywhere, Category = "Stats")
		float Damage = 50.0f;

	// Increase mana regen for gold
	UFUNCTION(BlueprintCallable)
		void BuyManaRegen();

	// Increase energy regen for gold
	UFUNCTION(BlueprintCallable)
		void BuyEnergyRegen();

	// Restore health for gold
	UFUNCTION(BlueprintCallable)
		void BuyHealth();

	// Buy an extra life
	UFUNCTION(BlueprintCallable)
		void BuyALife();

	// Cost to buy a life
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Shop")
		float LifeGoldCost = 1000.0f;

	// Buy more damage for the player's projectiles
	UFUNCTION(BlueprintCallable)
		void BuyDamage();

	// Cost to buy more mana regeneration
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop")
		int ManaGoldCost = 50;

	// Cost to buy more energy regeneration
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop")
		int EnergyGoldCost = 25;

	// Cost to restore health
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop")
		int HealthGoldCost = 50;

	// Cost to increase damage
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop")
		int DamageGoldCost = 400;

	// Spawn point for the projectiles
	UPROPERTY(EditAnywhere)
		USceneComponent* SparkSpawnpoint;

private:

	// Regenerate mana passively
	UFUNCTION()
		void RegenerateMana();

	// Regenerate energy passively
	UFUNCTION()
		void RegenerateEnergy();

	// Slowly drain gold from the player
	UFUNCTION()
		void StaticDrainGold();

	// Overridden takedamage function so the player can take damage on collision with slimes
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Dynamic delegate for collision
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Camera
	UPROPERTY(EditAnywhere)
		UCameraComponent* Camera;

	// Spring arm for camera
	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

	// Class of projectile to spawn
	UPROPERTY(EditAnywhere)
		TSubclassOf<ASparks> MagicClass;

	// Game mode reference
	UPROPERTY()
		AGamesFourGameModeBase* GameMode;



};
