#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CooldownWidget.generated.h"

UCLASS()
class TWILIGHTARCHERY_API UCooldownWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextJumpCooldown;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextDashCoolDown;
	UPROPERTY(meta = (BindWidget)) UProgressBar* JumpBarCoolDown;
	UPROPERTY(meta = (BindWidget)) UProgressBar* DashBarCoolDown;

	UPROPERTY(meta = (BindWidget)) UTextBlock* TextJumpCooldownTwo;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextDashCoolDownTwo;
	UPROPERTY(meta = (BindWidget)) UProgressBar* JumpBarCoolDownTwo;
	UPROPERTY(meta = (BindWidget)) UProgressBar* DashBarCoolDownTwo;
public:

	UFUNCTION(BlueprintCallable) void UpdateWidget            (const float& CurrTimer, const float& MaxTimer, const bool& PlayerTwo,const bool& IsDashing,const bool& IsJumping) const;
	UFUNCTION(BlueprintCallable) void SetVisibilityCooldownOn (const bool& PlayerTwo,const bool& IsDashing,const bool& IsJumping) const;
	UFUNCTION(BlueprintCallable) void SetVisibilityCooldownOff(const bool& PlayerTwo,const bool& IsDashing,const bool& IsJumping) const;
};
