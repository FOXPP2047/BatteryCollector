// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    
    // All pickups start active
    bIsActive = true;

    //Create the static Mesh Component
    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    RootComponent = PickupMesh;
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APickUp::IsActive() {
    return bIsActive;
}

void APickUp::SetActive(bool NewPickupState) {
    bIsActive = NewPickupState;
}
