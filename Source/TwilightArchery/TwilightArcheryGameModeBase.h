#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/TwilightArcheryPlayerController.h"
#include "TwilightArcheryGameModeBase.generated.h"

UCLASS()
class TWILIGHTARCHERY_API ATwilightArcheryGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	TArray<AActor*> PlayerStartArray;

	virtual void BeginPlay() override;

public:
	void SpawnPlayer(ATwilightArcheryPlayerController* Controller, const int& PlayerIndex) const;
};
