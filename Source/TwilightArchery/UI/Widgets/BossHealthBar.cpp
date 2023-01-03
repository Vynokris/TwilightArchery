#include "BossHealthBar.h"
#include "TwilightArchery/UI/GameHUD.h"

void UBossHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	HealthBarRight->SetPercent(100.f);
	HealthBarLeft ->SetPercent(100.f);
}

void UBossHealthBar::UpdateWidget(const float& Health, const float& MaxHealth) const
{
	HealthBarRight->SetPercent(Health / MaxHealth);
	HealthBarLeft ->SetPercent(Health / MaxHealth);
}
