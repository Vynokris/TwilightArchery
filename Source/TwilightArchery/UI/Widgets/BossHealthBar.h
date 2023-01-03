#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "BossHealthBar.generated.h"

UCLASS()
class TWILIGHTARCHERY_API UBossHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBarRight;
	UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBarLeft;

	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable) void UpdateWidget(const float& Health, const float& MaxHealth) const;
};
