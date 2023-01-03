#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "PlayerStaminaBar.generated.h"

UCLASS()
class TWILIGHTARCHERY_API UPlayerStaminaBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget)) UProgressBar* StaminaBar;
	UPROPERTY(meta = (BindWidget)) UProgressBar* StaminaBarTwo;

	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable) void UpdateWidget(const float& CurrStamina, const float& MaxStamina, const bool& SecondPlayer) const;
};
