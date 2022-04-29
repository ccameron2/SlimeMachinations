// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopPlatform.h"

// Sets default values
AShopPlatform::AShopPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMesh);

	ActivationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Activation Box"));
	ActivationBox->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AShopPlatform::BeginPlay()
{
	Super::BeginPlay();

	ActivationBox->OnComponentBeginOverlap.AddDynamic(this, &AShopPlatform::OnOverlapBegin);
	ActivationBox->OnComponentEndOverlap.AddDynamic(this, &AShopPlatform::OnOverlapEnd);
}

// Called every frame
void AShopPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AShopPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerPawn = Cast<AThirdPersonCharacter>(OtherActor);
}

void AShopPlatform::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}