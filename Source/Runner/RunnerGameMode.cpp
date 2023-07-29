// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerGameMode.h"
#include "IWidget.h"
#include "Runner/Assets/Floor.h"
#include "Blueprint/UserWidget.h"
#include"Runner/Character/SPlayer.h"

// creating the widget class and adding it to the viewport and
// spawning initial tiles 
void ARunnerGameMode::BeginPlay()
{
	Player = nullptr;
	if (WidgetClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		UI = CreateWidget<UIWidget>(PlayerController, WidgetClass);
		if (UI)
		{
			UI->AddToViewport();
		}
	}
	SpawnInititalTiles();
}

void ARunnerGameMode::SpawnInititalTiles()
{
	for (int i = 0; i < InitialSpawn; ++i)
		SpawnTile();
}

void ARunnerGameMode::SpawnPlayer()
{
	if(!Player)
	{
		Player = GetWorld()->SpawnActor<ASPlayer>(PlayerPawn, FVector(0.f, 0.f, 92.f), FRotator(0.f, 90.f, 0.f));
		GetWorld()->GetFirstPlayerController()->Possess(Player);
	}
}

void ARunnerGameMode::SpawnTile()
{
	if (GetWorld())
	{
		if (TotalFloors.Num() >= MaxFloorAtATime)
			return;

		AFloor* Floor = GetWorld()->SpawnActor<AFloor>(Tile, NextTileTransform);
		NextTileTransform = Floor->GetNextTileTransform();
		TotalFloors.Add(Floor);
	}
}

void ARunnerGameMode::RemoveFloor(AFloor* Floor)
{
	TotalFloors.Remove(Floor);
}

void ARunnerGameMode::UpdateTimeAndScore(float Time, int Score)
{
	if (UI)
		UI->UpdateUITimeAndScore(Time, Score);
}

void ARunnerGameMode::GameOver()
{
	if(UI)
	UI->GameOver();
}
