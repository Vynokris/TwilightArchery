#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerHealthBar.generated.h"

UCLASS()
class TWILIGHTARCHERY_API UPlayerHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBarTwo;

	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable) void UpdateWidget(const float& Health, const float& MaxHealth, const bool& SecondPlayer) const;
};
