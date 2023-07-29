// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportPickup.h"
#include "Runner/Character/SPlayer.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATeleportPickup::ATeleportPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("RootComp"));
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void ATeleportPickup::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATeleportPickup::BeginOverlap);
}

// Called every frame
void ATeleportPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldRotation(FRotator(0.f, RotationSpeed, 0.f));
}

void ATeleportPickup::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	Player = Cast<ASPlayer>(OtherActor);
	if (Player)
	{
		Player->HasTeleportBuff(true);
		Destroy();
	}
}

