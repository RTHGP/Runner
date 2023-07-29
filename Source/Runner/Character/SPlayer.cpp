// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayer.h"
#include "Runner/Assets/Floor.h"
#include "Runner/RunnerGameMode.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runner/Animation/PlayerAnimInstance.h"
#include "NiagaraComponent.h"

ASPlayer::ASPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creating the Default Components..
	RootComp = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleComp"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("MeshComp"));
	MeshComp->SetupAttachment(RootComp);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComp"));
	SpringArm->SetupAttachment(RootComp);

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("CameraComp"));
	Camera->SetupAttachment(SpringArm);

	Trail = CreateDefaultSubobject<UNiagaraComponent>(FName("TrailComp"));
	Trail->SetupAttachment(RootComp);
}

void ASPlayer::BeginPlay()
{
	Super::BeginPlay();

	//Sets the current speed to the default speed and,
	//stores the reference of my custom game mode class and,
	//initialize the game timer with playable time
	Speed = DefaultSpeed; 
	GM = Cast<ARunnerGameMode>(GetWorld()->GetAuthGameMode());
	if(GM)
	GameTimer = GM->GameTime;
	AnimInst = Cast<UPlayerAnimInstance>(MeshComp->GetAnimInstance());
	RootComp->OnComponentBeginOverlap.AddDynamic(this, &ASPlayer::BeginOverlap);
	if (Trail)
		Trail->SetActive(false);
}

void ASPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// if Game mode reference exists, then
	// update the game score and game time
	if (GM)
	{
		GM->UpdateTimeAndScore(GameTimer, (int)Score);
	}
	Score += DeltaTime * ScoreMultiplier * Speed;
	GameTimer -= DeltaTime;

	// after game time is finished
	// game is over.
	if (GameTimer <= 0.f)
	{
		//Game Over
		GM->GameOver();
		Destroy();
	}

	// apply teleport buff when acquired.
	// else, play default movement
	if(bHasTeleportBuff)
	{
		SetActorLocation(GetActorLocation() + FVector(0.f, TeleportSpeed * DeltaTime, 0.f));
		if (GetActorLocation().Y >= TargetLocation.Y)
		{
			RootComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
			if (Trail)
			Trail->SetActive(false);
			bHasTeleportBuff = false;
		}
	}
	else
	AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaTime, true);

	// executes jump logic and animation,
	// when, player is in jump state
	if (bIsJumped)
	{
		if (AnimInst && JumpMontage)
		{
			AnimInst->Montage_Play(JumpMontage);
		}
		UpdateJump(DeltaTime);
	}

	// apply sprint buff when acquired,
	// resets to default speed when buff is over.
	if (bHasSprintBuff)
	{
		SprintTimer += DeltaTime;
		if (SprintTimer >= 5.f)
		{
			SetSpeed(DefaultSpeed);
			HasSprintBuff(false);
			ResetSprintTimer();
		}
	}

}

void ASPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &ASPlayer::MoveRight);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &ASPlayer::MoveLeft);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASPlayer::BeginJump);

}

void ASPlayer::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{	
	// game over when crashed on an Obstacle
	if (OtherActor->ActorHasTag(FName("Obstacle")))
	{
		GM->GameOver();
		Destroy();
	}
	// check is player is on the ground,
	// if yes, then player is not in jump state
	if (OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel2)
		bIsJumped = false;
	// this is a trigger which handles,
	// spawning new floors and destroying the old ones.
	if(OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel3)
	{
		AFloor* Floor = Cast<AFloor>(OtherActor);
		GM->RemoveFloor(Floor);
		GM->SpawnTile();
	}
}		

void ASPlayer::MoveRight()
{
	if (bIsJumped || CurrentLane == RightLane)
		return;

	FVector DeltaVector = FVector(ShiftAmount, 0.f, 0.f);
	SetActorLocation(GetActorLocation() - DeltaVector);

	if (CurrentLane == LeftLane)
		CurrentLane = MiddleLane;
	else
		CurrentLane = RightLane;
}

void ASPlayer::MoveLeft()
{
	if (bIsJumped || CurrentLane == LeftLane)
		return;

	FVector DeltaVector = FVector(ShiftAmount, 0.f, 0.f);
	SetActorLocation(GetActorLocation() + DeltaVector);
	
	if (CurrentLane == RightLane)
		CurrentLane = MiddleLane;
	else
		CurrentLane = LeftLane;
}

void ASPlayer::BeginJump()
{
	if (bIsJumped) return;
	
	OldJumpVelocity = MaxJumpVelocity;
	bIsJumped = true;
}

void ASPlayer::UpdateJump(float DT)
{
	CurrentJumpVelocity = OldJumpVelocity - Gravity * DT;
	AddActorWorldOffset(FVector(0.f, 0.f, CurrentJumpVelocity), true);
	OldJumpVelocity = CurrentJumpVelocity;
}

float ASPlayer::GetSpeed()
{ 
	return Speed; 
}

void ASPlayer::ResetSprintTimer()
{
	SprintTimer = 0.f;
}

void ASPlayer::SetSpeed(float Value)
{
	Speed = Value;
}

void ASPlayer::HasSprintBuff(bool Value)
{
	bHasSprintBuff = Value;
}

void ASPlayer::HasTeleportBuff(bool Value)
{	
	if(Value)
	{
		RootComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
		if (Trail)
		Trail->SetActive(true);
	}
	bHasTeleportBuff = Value;
	TargetLocation = GetActorLocation() + FVector(0.f, 1000.f, 0.f);
	CurrentLocation = GetActorLocation();
}

