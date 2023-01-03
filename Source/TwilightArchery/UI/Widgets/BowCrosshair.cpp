#include "BowCrosshair.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Slate/SlateBrushAsset.h"
#include "TwilightArchery/Player/PlayerCharacter.h"

int32 UBowCrosshair::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext PaintContext = { AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled };

	if (CrosshairOutsideBrush && CrosshairCenterBrush && ArrowCountFont)
	{
		for (int i = 0; i < 2; i++)
		{
			// Get viewport center.
			FVector2D ViewportCenter = UWidgetLayoutLibrary::GetViewportSize(GetWorld()) / UWidgetLayoutLibrary::GetViewportScale(GetWorld()) / FVector2D(4, 2);
			ViewportCenter.X += ViewportCenter.X * 2 * i;

			// Draw the outer circle.
			float CircleSize = FMath::Lerp(CrosshairOutsideSizeMax, CrosshairOutsideSizeMin, AimingCompletion[i]);
			UWidgetBlueprintLibrary::DrawBox(PaintContext, ViewportCenter - CircleSize / 2, { CircleSize, CircleSize }, CrosshairOutsideBrush);

			// Draw the center point.
			UWidgetBlueprintLibrary::DrawBox(PaintContext, ViewportCenter - CrosshairCenterSize / 2, { CrosshairCenterSize, CrosshairCenterSize }, CrosshairCenterBrush);

			// Draw the number of arrows.
			const FString ArrowCountText = UKismetStringLibrary::Conv_IntToString(ArrowCount[i]);
			UWidgetBlueprintLibrary::DrawTextFormatted(PaintContext, FText::FromString(ArrowCountText), ViewportCenter - FVector2D(ArrowCountText.Len() * 6, -10), ArrowCountFont);
		}
	}
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void UBowCrosshair::UpdateWidget(const APlayerCharacter* Player)
{
	if (IsValid(Player))
	{
		const int        PlayerId  = Player->GetPlayerIndex();
		AimingCompletion[PlayerId] = Player->GetAimingCompletion();
		ArrowCount      [PlayerId] = Player->GetArrowCount      ();
	}
}
