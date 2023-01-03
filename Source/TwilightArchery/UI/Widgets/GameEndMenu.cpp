#include "GameEndMenu.h"

#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"

void UGameEndMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	RestartButton->OnClicked.AddDynamic(this, &UGameEndMenu::Restart);
	QuitButton   ->OnClicked.AddDynamic(this, &UGameEndMenu::Quit);
}

void UGameEndMenu::Restart() 
{
	GetWorld()->GetFirstPlayerController()->GetHUD()->Destroy();
	UGameplayStatics::OpenLevel(this,FName(UGameplayStatics::GetCurrentLevelName(GetWorld(),true)),true); 
}

void UGameEndMenu::Quit() 
{
	UGameplayStatics::OpenLevel(this, FName("MainMenu"), true);
}

void UGameEndMenu::ShowMenu()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		RestartButton->SetKeyboardFocus();		
		SetVisibility(ESlateVisibility::Visible);
	}
}
