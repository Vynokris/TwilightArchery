

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include  "Components/ProgressBar.h"
#include "ReviveBar.generated.h"

/**
 * 
 */
UCLASS()
class TWILIGHTARCHERY_API UReviveBar : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget)) UProgressBar* ReviveBar;
	
	virtual void NativeConstruct() override;
public:

	UFUNCTION(BlueprintCallable) void UpdateWidget(float CurrTime, float MaxTime);
	
};
