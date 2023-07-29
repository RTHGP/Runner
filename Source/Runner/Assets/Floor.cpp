// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include "Runner/Assets/Obstacle.h"
#include "Runner/Pickup/SprintPickup.h"
#include "Runner/Pickup/TeleportPickup.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComp"));
	SetRootComponent(Mesh);

	NextTileTrigger = CreateDefaultSubobject<UBoxComponent>(FName("TriggerComp"));
	NextTileTrigger->SetupAttachment(Mesh);

	NextFloor = CreateDefaultSubobject<UArrowComponent>(FName("NextTileComp"));
	NextFloor->SetupAttachment(Mesh);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(FName("LeftLane"));
	LeftLane->SetupAttachment(Mesh);
	
	MiddleLane = CreateDefaultSubobject<UArrowComponent>(FName("MiddleLane"));
	MiddleLane->SetupAttachment(Mesh);
	
	RightLane = CreateDefaultSubobject<UArrowComponent>(FName("RightLane"));
	RightLane->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	NextTileTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFloor::BeginOverlap);
	SpawnObstacle();
	SpawnPickups();
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bBeginDestroy)
	{
		LifeTime += DeltaTime;
		if (LifeTime >= TimeToDestroy)
		{
			for (AActor* Actor : Child)
			{
				if (Actor)
					Actor->Destroy();
				else
					Child.Remove(Actor);
			}
			Destroy();
		}
	}
}

void AFloor::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	bBeginDestroy = true;
}

void AFloor::SpawnObstacle()
{
	UWorld* World = GetWorld();
	if (!World) return;

	for (int i = 0; i < FMath::Rand() % 4; ++i)
	{
		int Lane = GetRandomLane();
		if (Lane == -1) return;

		AObstacle* Obs = nullptr;
		if (Lane == 0)
			Obs = World->SpawnActor<AObstacle>(Obstacle, LeftLane->GetComponentTransform());
		else if (Lane == 1)
			Obs = World->SpawnActor<AObstacle>(Obstacle, MiddleLane->GetComponentTransform());
		else if (Lane == 2)
			Obs = World->SpawnActor<AObstacle>(Obstacle, RightLane->GetComponentTransform());

		if (Obs)
		{
			FVector CorrectionVector = FVector(0.f, 0.f, 80.f);
			Obs->SetActorLocation(Obs->GetActorLocation() + CorrectionVector);
			Child.Add(Obs);
		}
	}
}

void AFloor::SpawnPickups()
{
	UWorld* World = GetWorld();
	if (!World) return;

	ASprintPickup* SP = nullptr;
	ATeleportPickup* TP = nullptr;

	int Lane = FMath::Rand() % 3;
	int Pickup = FMath::Rand() % 2;
	if (Pickup == 0)
	{
		if (Lane == 0)
			SP = World->SpawnActor<ASprintPickup>(SprintPickup, LeftLane->GetComponentTransform());
		else if (Lane == 1)
			SP = World->SpawnActor<ASprintPickup>(SprintPickup, MiddleLane->GetComponentTransform());
		else if (Lane == 2)
			SP = World->SpawnActor<ASprintPickup>(SprintPickup, RightLane->GetComponentTransform());

		if (SP)
		{
			FVector CorrectionVector;
			int Location = FMath::Rand() % 2;
			if (Location == 0)
				CorrectionVector = FVector(0.f, FMath::FRandRange(100.f, 200.f), 80.f);
			else
				CorrectionVector = FVector(0.f, FMath::FRandRange(-200.f, -100.f), 80.f);
			SP->SetActorLocation(SP->GetActorLocation() + CorrectionVector);
			Child.Add(SP);
		}
	}
	else
	{
		if (Lane == 0)
			TP = World->SpawnActor<ATeleportPickup>(TeleportPickup, LeftLane->GetComponentTransform());
		else if (Lane == 1)
			TP = World->SpawnActor<ATeleportPickup>(TeleportPickup, MiddleLane->GetComponentTransform());
		else if (Lane == 2)
			TP = World->SpawnActor<ATeleportPickup>(TeleportPickup, RightLane->GetComponentTransform());

		if (TP)
		{
			FVector CorrectionVector;
			int Location = FMath::Rand() % 2;
			if (Location == 0)
				CorrectionVector = FVector(0.f, FMath::FRandRange(100.f, 200.f), 80.f);
			else
				CorrectionVector = FVector(0.f, FMath::FRandRange(-200.f, -100.f), 80.f);
			TP->SetActorLocation(TP->GetActorLocation() + CorrectionVector);
			Child.Add(TP);
		}
	}
}

int AFloor::GetRandomLane()
{
	if (UsedLane == FilledLane) return -1;
	int Lane = FMath::Rand() % 3;
	while (true)
	{
		if (UsedLane & LANE_BIT(Lane)) {
			Lane = FMath::Rand() % 3;
			continue;
		}
		UsedLane = UsedLane | LANE_BIT(Lane);
		break;
	}
	return Lane;
}

FTransform AFloor::GetNextTileTransform()
{
	return NextFloor->GetComponentTransform();
}

