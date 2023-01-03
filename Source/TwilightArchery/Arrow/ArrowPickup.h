#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowPickup.generated.h"
class AArrowPickup;

DECLARE_MULTICAST_DELEGATE_OneParam(FPickupEmptySignature, AArrowPickup*);

UCLASS()
class TWILIGHTARCHERY_API AArrowPickup : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Arrow Pickup Components") UShapeComponent*      BoxComponent  = nullptr;
	UPROPERTY(EditAnywhere, Category = "Arrow Pickup Components") UStaticMeshComponent* MeshComponent = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Arrow Pickup Properties") int PickupArrowsCount = 5;
	UPROPERTY(EditAnywhere, Category = "Arrow Pickup Properties") int MaxNumberOfUses   = 1;

	UPROPERTY(EditAnywhere, Category = "Arrow Pickup Properties") int StaminaAddedOnPickup   = 10;

	int NumberOfUses = 0;
	
public:
	AArrowPickup();
	FPickupEmptySignature PickupEmptyDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
