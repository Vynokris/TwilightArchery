#include "GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "TwilightArchery/Player/PlayerCharacter.h"
#include "TwilightArchery/Utils/DebugUtils.h"

void AGameHUD::DrawHUD()
{
	LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(),true);
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	const APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwningPlayerController()->GetPawn());
	
	if (!IsValid(OwningPlayer) || GetOwningPlayerController() != GetWorld()->GetFirstPlayerController()) { Destroy(); return; }

	
	if (PlayerHealthBarBP)
	{
		PlayerHealthBar = CreateWidget<UPlayerHealthBar>(GetWorld(), PlayerHealthBarBP);
		if (PlayerHealthBar) PlayerHealthBar->AddToViewport(1);
	}

	if (PlayerStaminaBarBP)
	{
		PlayerStaminaBar = CreateWidget<UPlayerStaminaBar>(GetWorld(), PlayerStaminaBarBP);
		if (PlayerStaminaBar) PlayerStaminaBar->AddToViewport(1);
	}

	if (PlayerScoreWidgetBP)
	{
		PlayerScoreWidget = CreateWidget<UPlayerScoreWidget>(GetWorld(), PlayerScoreWidgetBP);
		if (PlayerScoreWidget) PlayerScoreWidget->AddToViewport(1);
	}

	if (BowCrosshairWidgetBP)
	{
		BowCrosshairWidget = CreateWidget<UBowCrosshair>(GetWorld(), BowCrosshairWidgetBP);
		if (BowCrosshairWidget) BowCrosshairWidget->AddToViewport(1);
	}
	
	if (BossHealthBarBP)
	{
		BossHealthBar = CreateWidget<UBossHealthBar>(GetWorld(), BossHealthBarBP);
		if (BossHealthBar)
		{
			BossHealthBar->AddToViewport(1);
			BossHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	if (PauseMenuBP)
	{
		PauseMenu = CreateWidget<UPauseMenu>(GetWorld(), PauseMenuBP);
		if (PauseMenu)
		{
			PauseMenu->AddToViewport(3);
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	if (GameOverMenuBP)
	{
		GameOverMenu = CreateWidget<UGameEndMenu>(GetWorld(), GameOverMenuBP);
		if (GameOverMenu)
		{
			GameOverMenu->AddToViewport(4);
			GameOverMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	if (VictoryMenuBP)
	{
		VictoryMenu = CreateWidget<UGameEndMenu>(GetWorld(), VictoryMenuBP);
		if (VictoryMenu)
		{
			VictoryMenu->AddToViewport(4);
			VictoryMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	if (ReviveBarBP)
	{
		ReviveBar = CreateWidget<UReviveBar>(GetWorld(), ReviveBarBP);
		if (ReviveBar)
		{
			ReviveBar->AddToViewport(2);
			ReviveBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	if (CooldownWidgetBP)
	{
		CooldownWidget = CreateWidget<UCooldownWidget>(GetWorld(), CooldownWidgetBP);
		if (CooldownWidget)
		{
			CooldownWidget->AddToViewport(1);
		}
	}
}
