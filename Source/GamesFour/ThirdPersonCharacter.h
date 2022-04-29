// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Sparks.h"
#include "GamesFourGameModeBase.h"
#include "Components/ActorComponent.h" 

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

	void MoveForward(float AxisValue);
	void Strafe(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);
	void Fire();
	void Jump();

	UPROPERTY(EditAnywhere)
		float MoveSpeed = 200.0f;

	UPROPERTY(EditAnywhere)
		float RotationSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxEnergy = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxExp = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxMana = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxGold = 999.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UpgradeAmount = 20.0f;

	UFUNCTION(BlueprintCallable)
		void LevelUp();

	UFUNCTION(BlueprintCallable)
		bool HasSkillPoints();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float HealthPoints = MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float ExpPoints = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Energy = MaxEnergy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Gold = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Mana = MaxMana;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Lives = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int SkillPoints = 0;

	UPROPERTY(EditAnywhere)
		float ManaCost = 10.0f;

	UPROPERTY(EditAnywhere)
		float ManaRegenTime = 2.0f;

	UPROPERTY(EditAnywhere)
		float ManaRegenAmount = 10.0f;

	UPROPERTY(EditAnywhere)
		float EnergyRegenTime = 1.0f;

	UPROPERTY(EditAnywhere)
		float EnergyRegenAmount = 2.0f;

	UPROPERTY(EditAnywhere)
		float GoldDrainTime = 1.0f;

	UFUNCTION(BlueprintCallable)
		void BuyManaRegen();

	UFUNCTION(BlueprintCallable)
		void BuyEnergyRegen();

	UFUNCTION(BlueprintCallable)
		void BuyHealth();

	UPROPERTY(EditAnywhere)
		float Damage = 50.0f;

	UFUNCTION(BlueprintCallable)
		void BuyDamage();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int ManaGoldCost = 50;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int EnergyGoldCost = 25;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int HealthGoldCost = 50;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int DamageGoldCost = 400;

private:

	UFUNCTION()
		void RegenerateMana();

	UFUNCTION()
		void RegenerateEnergy();

	UFUNCTION()
		void StaticDrainGold();

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASparks> MagicClass;

	UPROPERTY()
		AGamesFourGameModeBase* GameMode;

	UPROPERTY(EditAnywhere)
		UActorComponent* SparkSpawnpoint;

};
