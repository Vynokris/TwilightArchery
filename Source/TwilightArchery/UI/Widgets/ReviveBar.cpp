


#include "ReviveBar.h"

#include "GenericPlatform/GenericPlatformCrashContext.h"

void UReviveBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UReviveBar::UpdateWidget(float CurrTime, float MaxTime)
{
	ReviveBar->SetPercent(CurrTime/MaxTime);
}



