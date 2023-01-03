#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerScoreWidget.generated.h"

UCLASS()
class TWILIGHTARCHERY_API UPlayerScoreWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget)) UTextBlock* ScoreText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ScoreTextTwo;

public:
	UFUNCTION(BlueprintCallable) void UpdateWidget(const int& Score, const bool& SecondPlayer) const;
};
