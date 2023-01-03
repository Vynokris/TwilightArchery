#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Kismet/GameplayStatics.h"
#include "BTT_BossShoot.generated.h"
class ABoss;

UCLASS(Blueprintable)
class TWILIGHTARCHERY_API UBTT_BossShoot : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere) UClass* BarragePattern = nullptr;
	ABoss* Boss              = nullptr;
	float  HeightAboveGround = 0;

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	UPROPERTY(EditAnywhere) USoundBase* Sound = nullptr;
};

