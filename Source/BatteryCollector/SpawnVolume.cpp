// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "PickUp.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
    RootComponent = WhereToSpawn;

    //Set the spawn delay range
    SpawnDelayRangeLow = 1.0f;
    SpawnDelayRangeHigh = 4.5f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
    SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
    GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, true);
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomPointInVolume() {
    FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
    FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

    return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void ASpawnVolume::SpawnPickup() {
    if (WhatToSpawn != NULL) {
        UWorld* const world = GetWorld();

        if (world) {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            FVector SpawnLocation = GetRandomPointInVolume();

            FRotator SpawnRotation;
            SpawnRotation.Yaw = FMath::FRand() * 360.0f;
            SpawnRotation.Pitch = FMath::FRand() * 360.0f;
            SpawnRotation.Roll = FMath::FRand() * 360.0f;

            APickUp* const SpawnPickup = world->SpawnActor<APickUp>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
        }
    }
}

