// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IWidget.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UIWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* Widget_Switcher;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_Quit;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_Restart;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_End;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* T_Timer;
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* T_Score;
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* T_Final;

	class ARunnerGameMode* GM;

	bool Initialize() override;

	void NativeConstruct() override;

	UFUNCTION()
		// run when start button is pressed
		void Start();

	UFUNCTION()
		// run when quit and end button is pressed
		void Quit();

	UFUNCTION()
		// run when restart button is pressed
		void Restart();

public:

	// updates remaining playable time and current score
	void UpdateUITimeAndScore(float Time, int Score);

	// opens gameover menu
	void GameOver();
};
