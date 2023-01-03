#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowCrosshair.generated.h"
class USlateBrushAsset;
class APlayerCharacter;

UCLASS()
class TWILIGHTARCHERY_API UBowCrosshair : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Properties") USlateBrushAsset* CrosshairOutsideBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Properties") USlateBrushAsset* CrosshairCenterBrush;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Properties") UFont*            ArrowCountFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Properties") float CrosshairOutsideSizeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Properties") float CrosshairOutsideSizeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Properties") float CrosshairCenterSize;
	
	float AimingCompletion[2] = { 0,  0  };
	int   ArrowCount      [2] = { 15, 15 };
	
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

public:
	UFUNCTION(BlueprintCallable) void UpdateWidget(const APlayerCharacter* Player);
};
