// Fill out your copyright notice in the Description page of Project Settings.

#include "Obstacle.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));
	SetRootComponent(BoxComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComp"));
	Mesh->SetupAttachment(BoxComp);
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	this->Tags.Add(FName("Obstacle"));
}