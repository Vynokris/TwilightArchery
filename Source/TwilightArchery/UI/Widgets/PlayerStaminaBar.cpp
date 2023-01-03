#include "PlayerStaminaBar.h"
#include "TwilightArchery/UI/GameHUD.h"

void UPlayerStaminaBar::NativeConstruct()
{
	Super::NativeConstruct();
	StaminaBar   ->SetPercent(100.f);
	StaminaBarTwo->SetPercent(100.f);
}

void UPlayerStaminaBar::UpdateWidget(const float& Stamina, const float& MaxStamina, const bool& SecondPlayer) const
{
	if (SecondPlayer) StaminaBarTwo->SetPercent(Stamina / MaxStamina);
	else              StaminaBar   ->SetPercent(Stamina / MaxStamina);
}
