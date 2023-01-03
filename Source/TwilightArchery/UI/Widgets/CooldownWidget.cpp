#include "CooldownWidget.h"

void UCooldownWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TextJumpCooldown   ->SetVisibility(ESlateVisibility::Hidden);
	TextDashCoolDown   ->SetVisibility(ESlateVisibility::Hidden);
	TextJumpCooldownTwo->SetVisibility(ESlateVisibility::Hidden);
	TextDashCoolDownTwo->SetVisibility(ESlateVisibility::Hidden);
}

void UCooldownWidget::UpdateWidget(const float& CurrTimer, const float& MaxTimer, const bool& PlayerTwo, const bool& IsDashing, const bool& IsJumping) const
{
	if (PlayerTwo)
	{
		if (IsDashing) {
			DashBarCoolDownTwo ->SetPercent(CurrTimer/MaxTimer);
			TextDashCoolDownTwo->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), CurrTimer)));
		}
		else if (IsJumping) {
			JumpBarCoolDownTwo ->SetPercent(CurrTimer/MaxTimer);
			TextJumpCooldownTwo->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), CurrTimer)));
		}
	}
	else
	{
		if (IsDashing) {
			DashBarCoolDown ->SetPercent(CurrTimer/MaxTimer);
			TextDashCoolDown->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), CurrTimer)));
		}
		if (IsJumping) {
			JumpBarCoolDown ->SetPercent(CurrTimer/MaxTimer);
			TextJumpCooldown->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), CurrTimer)));
		}
	}
}

void UCooldownWidget::SetVisibilityCooldownOn(const bool& PlayerTwo, const bool& IsDashing, const bool& IsJumping) const
{
	if (PlayerTwo)
	{
		if (IsDashing) {
			TextDashCoolDownTwo->SetVisibility(ESlateVisibility::Visible);
		}
		if(IsJumping) {
			TextJumpCooldownTwo->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (IsDashing) {
			TextDashCoolDown->SetVisibility(ESlateVisibility::Visible);
		}
		if (IsJumping){
			TextJumpCooldown->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UCooldownWidget::SetVisibilityCooldownOff(const bool& PlayerTwo, const bool& IsDashing, const bool& IsJumping) const
{
	if(PlayerTwo)
	{
		if (IsDashing) {
			DashBarCoolDownTwo ->SetPercent(0);
			TextDashCoolDownTwo->SetVisibility(ESlateVisibility::Hidden);
		}
		if (IsJumping) {
			JumpBarCoolDownTwo ->SetPercent(0);
			TextJumpCooldownTwo->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		if (IsDashing) {
			DashBarCoolDown ->SetPercent(0);
			TextDashCoolDown->SetVisibility(ESlateVisibility::Hidden);
		}
		if (IsJumping) {
			JumpBarCoolDown ->SetPercent(0);
			TextJumpCooldown->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


