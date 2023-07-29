// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	void NativeBeginPlay() override;

	// updates running animation speed
	void NativeUpdateAnimation(float DeltaTime) override;

private:

	UPROPERTY(BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = true))
	float Speed;

	class ASPlayer* Player;
};
