#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TwilightArchery/Utils/Cooldown.h"
#include "ArrowPickupSpawner.generated.h"
class ATriggerBox;
class AArrowPickup;

UCLASS()
class TWILIGHTARCHERY_API AArrowPickupSpawner : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Arrow Pickup Spawner Properties") TSubclassOf<AArrowPickup> ArrowPickupBP           = nullptr;
	UPROPERTY(EditAnywhere, Category = "Arrow Pickup Spawner Properties") int                       MaxActiveArrowPickups   = 5;
	UPROPERTY(EditAnywhere, Category = "Arrow Pickup Spawner Properties") float                     RespawnCooldownDuration = 3;

	UShapeComponent* BoxComponent = nullptr;
	TArray<AArrowPickup*> ArrowPickups;
	FCooldown RespawnCooldown = { 0 };
	FVector SpawnOrigin;
	FVector SpawnExtent;
	
public:	
	AArrowPickupSpawner();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void SpawnArrowPickup();
};
