// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

#define LANE_BIT(x) 1<<x

UCLASS()
class RUNNER_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UArrowComponent* NextFloor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UArrowComponent* LeftLane;
	
	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UArrowComponent* MiddleLane;
	
	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UArrowComponent* RightLane;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UBoxComponent* NextTileTrigger;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class AObstacle> Obstacle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class ASprintPickup> SprintPickup;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class ATeleportPickup> TeleportPickup;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		float TimeToDestroy = 2.f;

	UFUNCTION()
	// when triggered, will
	// set destroy status for this floor object.
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	// spawns objstacle on this floor
	// adds random number of obstacle to empty lanes
	void SpawnObstacle();

	// spwans buff pickups on this floor
	void SpawnPickups();

	// returns, a random empty lane
	// else, returns -1
	int GetRandomLane();

	TArray<AActor*> Child;

	bool bBeginDestroy = false;

	int UsedLane = 0;

	const int FilledLane = (LANE_BIT(0) | LANE_BIT(1) | LANE_BIT(2));

	float LifeTime = 0.f;

public:

	FTransform GetNextTileTransform();
};
