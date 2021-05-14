// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
private:
    void SpawnPickup();
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    //Returns the WhereToSpawn subObject
    FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

    //Find a random point within the Boxcomponent
    UFUNCTION(BlueprintPure, Category = "Spawning")
    FVector GetRandomPointInVolume();

    //This function toggles whether or not the spawn volume spawns pickups
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SetSpawingActive(bool bShouldSpawn);
private:
    //Box Component to specify where pickups should be spawned
    UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* WhereToSpawn;

    float SpawnDelay;
protected:
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<class APickUp> WhatToSpawn;

    FTimerHandle SpawnTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnDelayRangeLow;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnDelayRangeHigh;
};
