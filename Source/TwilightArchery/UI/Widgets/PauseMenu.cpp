#include "PauseMenu.h"

#include "BowCrosshair.h"
#include "TwilightArchery/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TwilightArchery/UI/GameHUD.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
	ResumeButton ->OnClicked.AddDynamic(this, &UPauseMenu::Resume);
	RestartButton->OnClicked.AddDynamic(this, &UPauseMenu::Restart);
	QuitButton   ->OnClicked.AddDynamic(this, &UPauseMenu::Quit);
}

void UPauseMenu::Pause()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	Cast<APlayerCharacter>(PlayerController->GetPawn())->GetGameHud()->PauseMenu->SetVisibility(ESlateVisibility::Visible);
	PlayerController->SetInputMode(FInputModeUIOnly());
	ResumeButton->SetKeyboardFocus();
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetPause(true);
}

void UPauseMenu::Resume()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	Cast<APlayerCharacter>(PlayerController->GetPawn())->GetGameHud()->PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetPause(false);
}

void UPauseMenu::Restart()
{
	GetWorld()->GetFirstPlayerController()->GetHUD()->Destroy();
	UGameplayStatics::OpenLevel(this,FName(UGameplayStatics::GetCurrentLevelName(GetWorld(),true)),true); 
}

void UPauseMenu::Quit()
{
	UGameplayStatics::OpenLevel(this,FName("MainMenu"),true); 
}


