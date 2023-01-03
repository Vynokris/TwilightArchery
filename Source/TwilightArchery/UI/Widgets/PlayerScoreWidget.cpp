#include "PlayerScoreWidget.h"
#include "Kismet/KismetTextLibrary.h"

void UPlayerScoreWidget::UpdateWidget(const int& Score, const bool& SecondPlayer) const
{
	if (SecondPlayer) ScoreTextTwo->SetText(FText::FromString(FString("Score: ") + FString::FromInt(Score)));
	else              ScoreText   ->SetText(FText::FromString(FString("Score: ") + FString::FromInt(Score)));
}
