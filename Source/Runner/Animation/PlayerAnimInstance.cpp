// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "Runner/Character/SPlayer.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	Player = Cast<ASPlayer>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if(!Player)
	{
		Player = Cast<ASPlayer>(TryGetPawnOwner());
		return;
	}

	Speed = Player->GetSpeed();

}