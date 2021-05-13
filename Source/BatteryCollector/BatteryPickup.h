// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUp.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickUp
{
	GENERATED_BODY()
public:
    //Set default value
    ABatteryPickup();

    //Override the WasCollected function - use Implementation because it's a Blueprint Native Event
    virtual void WasCollected_Implementation() override;

    //Public way to access the battery's power level
    float GetPower();
protected:
    //Set the amount of power the battery gives to the character.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
    float BatteryPower;
};
