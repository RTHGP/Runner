// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprintPickup.generated.h"

UCLASS()
class RUNNER_API ASprintPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprintPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		float BuffedSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		float RotationSpeed = 10.f;

	UFUNCTION()
		// when triggered, provides sprint buff to the player
		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	class ASPlayer* Player;
};
