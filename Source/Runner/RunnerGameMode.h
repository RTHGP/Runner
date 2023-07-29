// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RunnerGameMode.generated.h"

/**
 * This class acts as a game manager for tis runner game.
 */
UCLASS()
class RUNNER_API ARunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class AFloor> Tile;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class ASPlayer> PlayerPawn;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int InitialSpawn = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int MaxFloorAtATime = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float GameTime = 180.f;

	UPROPERTY()
		class UIWidget* UI;

private:

	class ASPlayer* Player;

	FTransform NextTileTransform = FTransform();

	TArray<AFloor*> TotalFloors;

	void SpawnInititalTiles();

public:

	// spawns player in current scene
	void SpawnPlayer();

	// spawns floor tiles in current scene
	void SpawnTile();

	// remove floor from the floor array
	void RemoveFloor(AFloor* Floor);

	// provides remaining time and currentscore to game UI.
	void UpdateTimeAndScore(float Time, int Score);

	// calls to UI gameover menu
	void GameOver();

};
