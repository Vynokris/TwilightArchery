#include "PlayerHealthBar.h"
#include "TwilightArchery/UI/GameHUD.h"

void UPlayerHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	HealthBar   ->SetPercent(100.f);
	HealthBarTwo->SetPercent(100.f);
}

void UPlayerHealthBar::UpdateWidget(const float& Health, const float& MaxHealth, const bool& SecondPlayer) const
{
	if(SecondPlayer) HealthBarTwo->SetPercent(Health / MaxHealth);
	else             HealthBar   ->SetPercent(Health / MaxHealth);
}


