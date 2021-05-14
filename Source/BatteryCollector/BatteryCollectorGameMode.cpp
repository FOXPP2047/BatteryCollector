// Copyright Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"
#include "SpawnVolume.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    PrimaryActorTick.bCanEverTick = true;
    //base deacy rate
    DecayRate = 0.01f;
}

void ABatteryCollectorGameMode::BeginPlay() {
    Super::BeginPlay();

    //Find all spawn volume Actors
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

    for (auto Actor : FoundActors) {
        ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);

        if (SpawnVolumeActor) {
            SpawnVolumeActors.AddUnique(SpawnVolumeActor);
        }
    }

    SetCurrentState(EBatteryPlayState::EPlaying);
    //set the score to beat
    ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

    if (MyCharacter) {
        PowerToWin = MyCharacter->GetInitialPower() * 1.25f;
    }

    if (HUDWidgetClass != nullptr) {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

        if (CurrentWidget != nullptr) {
            CurrentWidget->AddToViewport();
        }
    }
}

void ABatteryCollectorGameMode::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

    if (MyCharacter) {
        if (MyCharacter->GetCurrentPower() > PowerToWin) {
            SetCurrentState(EBatteryPlayState::EWon);
        } else if (MyCharacter->GetCurrentPower() > 0) {
            MyCharacter->UpdatePower(-DeltaTime * DecayRate * MyCharacter->GetInitialPower());
        }
        else {
            SetCurrentState(EBatteryPlayState::EGameOver);
        }
    }
}

float ABatteryCollectorGameMode::GetPowerToWin() const {
    return PowerToWin;
}

EBatteryPlayState ABatteryCollectorGameMode::GetCurrentState() const {
    return CurrentState;
}

//Sets a new playing state
void ABatteryCollectorGameMode::SetCurrentState(EBatteryPlayState NewState) {
    CurrentState = NewState;
    HandleCurrentState(CurrentState);
}

void ABatteryCollectorGameMode::HandleCurrentState(EBatteryPlayState NewState) {
    switch (NewState) {
        case EBatteryPlayState::EPlaying:
        {
            //spawn volumes active
            for (auto volume : SpawnVolumeActors) {
                volume->SetSpawingActive(true);
            }
        }
        break;
        case EBatteryPlayState::EWon:
        {
            //spawn volumes inactive
            for (auto volume : SpawnVolumeActors) {
                volume->SetSpawingActive(false);
            }
        }
        break;
        case EBatteryPlayState::EGameOver:
        {
            //spawn volumes inactive
            for (auto volume : SpawnVolumeActors) {
                volume->SetSpawingActive(false);
            }
            //Block Player's input
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

            if (PlayerController) {
                PlayerController->SetCinematicMode(true, false, false, true, true);
            }
            //Ragdoll the character
            ACharacter* MyCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

            if (MyCharacter) {
                MyCharacter->GetMesh()->SetSimulatePhysics(true);
                MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
            }
        }
        break;
        default:
        case EBatteryPlayState::EUnknown:
        {

        }
        break;
    }
}