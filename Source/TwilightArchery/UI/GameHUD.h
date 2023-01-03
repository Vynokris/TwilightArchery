#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/PlayerHealthBar.h"
#include "Widgets/PlayerStaminaBar.h"
#include "Widgets/PlayerScoreWidget.h"
#include "Widgets/BossHealthBar.h"
#include "Widgets/BowCrosshair.h"
#include "Widgets/PauseMenu.h"
#include "Widgets/GameEndMenu.h"
#include "Widgets/ReviveBar.h"
#include "Widgets/CooldownWidget.h"
#include "GameHUD.generated.h"

UCLASS()
class TWILIGHTARCHERY_API AGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widget") TSubclassOf<UPlayerHealthBar  > PlayerHealthBarBP;
	UPROPERTY(EditDefaultsOnly, Category = "Widget") TSubclassOf<UPlayerStaminaBar > PlayerStaminaBarBP;
	UPROPERTY(EditDefaultsOnly, Category = "Widget") TSubclassOf<UPlayerScoreWidget> PlayerScoreWidgetBP;
	UPROPERTY(EditDefaultsOnly, Category = "Widget") TSubclassOf<UBowCrosshair     > BowCrosshairWidgetBP;
	UPROPERTY(EditDefaultsOnly, Category = "Widget") TSubclassOf<UBossHealthBar    > BossHealthBarBP;
	UPROPERTY(EditDefaultsOnly, Category = "Widget") TSubclassOf<UPauseMenu        > PauseMenuBP;
	UPROPERTY(EditDefaultsOnly, Category = "Widget") TSubclassOf<UGameEndMenu      > GameOverMenuBP;
	UPROPERTY(EditDefaultsOnly, Category = "Widget") TSubclassOf<UGameEndMenu      > VictoryMenuBP;
	UPROPERTY(EditDefaultsOnly, Category = "Widget") TSubclassOf<UReviveBar        > ReviveBarBP;
	UPROPERTY(EditDefaultsOnly, Category = "Widget") TSubclassOf<UCooldownWidget   > CooldownWidgetBP;

	virtual void DrawHUD() override;
	virtual void BeginPlay() override;
	
public:
	FString LevelName;
	
	UPlayerHealthBar*   PlayerHealthBar    = nullptr;
	UPlayerStaminaBar*  PlayerStaminaBar   = nullptr;
	UPlayerScoreWidget* PlayerScoreWidget  = nullptr;
	UBowCrosshair*      BowCrosshairWidget = nullptr;
	UBossHealthBar*     BossHealthBar      = nullptr;
	UPauseMenu*         PauseMenu          = nullptr;
	UGameEndMenu*       GameOverMenu       = nullptr;
	UGameEndMenu*       VictoryMenu        = nullptr;
	UReviveBar*         ReviveBar          = nullptr;
	UCooldownWidget*    CooldownWidget     = nullptr;
};