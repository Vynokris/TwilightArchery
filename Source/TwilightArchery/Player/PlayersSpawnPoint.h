#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PlayersSpawnPoint.generated.h"

UCLASS()
class TWILIGHTARCHERY_API APlayersSpawnPoint : public APlayerStart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int SpawnId;

	void BeginPlay() override;
};
