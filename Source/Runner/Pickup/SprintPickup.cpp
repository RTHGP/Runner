// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintPickup.h"
#include "Runner/Character/SPlayer.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASprintPickup::ASprintPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("RootComp"));
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void ASprintPickup::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASprintPickup::BeginOverlap);
}

// Called every frame
void ASprintPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldRotation(FRotator(0.f, RotationSpeed, 0.f));
}

void ASprintPickup::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	// removes any previous sprintbuff, if any
	// and adds a new buff with new timer
	Player = Cast<ASPlayer>(OtherActor);
	if (Player)
	{
		Player->ResetSprintTimer();
		Player->SetSpeed(BuffedSpeed);
		Player->HasSprintBuff(true);
		Destroy();
	}
}

