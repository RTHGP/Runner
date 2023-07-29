// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SPlayer.generated.h"

enum Lane {
	LeftLane = 0,
	MiddleLane = 1,
	RightLane = 2,
};

UCLASS()
class RUNNER_API ASPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Handles Player Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UAnimMontage* JumpMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		class UNiagaraComponent* Trail;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		float DefaultSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		float ShiftAmount = 330.f;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		float MaxJumpVelocity = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		float Gravity = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		float TeleportSpeed = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (AllowPrivateAccess = true))
		float ScoreMultiplier = 0.5f;

	UFUNCTION()
		// Handles collision event logics
		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	// implements the right movement logic,
	// moves to right only when a lane is available,
	// and is not in jump state.
	void MoveRight();

	// implements the left movement logic,
	// moves to left only when a lane is available,
	// and is not in jump state.
	void MoveLeft();

	// sets player in jump state, 
	// and, provides initial jump velocity to player
	void BeginJump();

	// updates jump velocity and player position per frame
	void UpdateJump(float DT);

	class ARunnerGameMode* GM;

	class UPlayerAnimInstance* AnimInst;

	enum Lane CurrentLane = MiddleLane;

	FVector CurrentLocation;

	FVector TargetLocation;

	bool bIsJumped = false;

	bool bHasSprintBuff = false;

	bool bHasTeleportBuff = false;

	float CurrentJumpVelocity = 0.f;

	float OldJumpVelocity = 0.f;

	float Speed = 0.f;

	float SprintTimer = 0.f;

	float GameTimer = 0.f;

	double Score = 0;

public:

	// returns, player current speed
	float GetSpeed();

	// resets sprint timer
	void ResetSprintTimer();

	// sets player speed
	void SetSpeed(float Value);

	// sets player sprint buff status
	void HasSprintBuff(bool Value);

	// sets player teleport buff status
	void HasTeleportBuff(bool Value);
};
