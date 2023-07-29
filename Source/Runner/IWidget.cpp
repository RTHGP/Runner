// Fill out your copyright notice in the Description page of Project Settings.


#include "IWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "RunnerGameMode.h"

// setting the maine menu widget when starting the game
bool UIWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	GM = Cast<ARunnerGameMode>(GetWorld()->GetAuthGameMode());
	if(!bSuccess || !Btn_Quit || !Btn_Start || !GM)
	return false;


	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FInputModeUIOnly UIMode;
	UIMode.SetWidgetToFocus(TakeWidget());
	PlayerController->SetInputMode(UIMode);
	PlayerController->bShowMouseCursor = true;
	Widget_Switcher->SetActiveWidgetIndex(0);
	return true;
}

// binding the function to the buttons onclick delegate
void UIWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(Btn_Start)
		Btn_Start->OnClicked.AddDynamic(this, &UIWidget::Start);

	if(Btn_Quit)
		Btn_Quit->OnClicked.AddDynamic(this, &UIWidget::Quit);

	if (Btn_Restart)
		Btn_Restart->OnClicked.AddDynamic(this, &UIWidget::Restart);

	if (Btn_End)
		Btn_End->OnClicked.AddDynamic(this, &UIWidget::Quit);
}

void UIWidget::Start()
{
	if(GM)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		FInputModeGameOnly GameOnlyMode;
		PlayerController->SetInputMode(GameOnlyMode);
		PlayerController->bShowMouseCursor = false;
		Widget_Switcher->SetActiveWidgetIndex(1);
		GM->SpawnPlayer();
	}
}

void UIWidget::Quit()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
		PlayerController->ConsoleCommand("quit");
}

void UIWidget::Restart()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
		PlayerController->ConsoleCommand("RestartLevel");
}

void UIWidget::UpdateUITimeAndScore(float Time, int Score)
{	
	T_Timer->SetText(FText::AsNumber((int)Time));
	T_Score->SetText(FText::AsNumber(Score));
	T_Final->SetText(T_Score->GetText());
}

void UIWidget::GameOver()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FInputModeUIOnly UIMode;
	UIMode.SetWidgetToFocus(TakeWidget());
	PlayerController->SetInputMode(UIMode);
	PlayerController->bShowMouseCursor = true;
	Widget_Switcher->SetActiveWidgetIndex(2);
}
