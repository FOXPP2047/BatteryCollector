// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    //Return the mesh for the pickup
    FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

    //Return whether or not the pickup is active
    UFUNCTION(BlueprintPure, Category = "Pickup")
    bool IsActive();

    //Allows other classes to safely change whether or not pickup is deactivated
    UFUNCTION(BlueprintCallable, Category = "Pickup")
    void SetActive(bool NewPickupState);
private:
    //Static Mesh to represent the pickup in the level
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* PickupMesh;
protected:
    //True when the pickup can be used, and false when pickup is deactivated.
    bool bIsActive;
};
