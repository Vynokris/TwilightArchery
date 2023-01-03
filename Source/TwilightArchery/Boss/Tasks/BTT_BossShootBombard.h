#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Kismet/GameplayStatics.h"
#include "BTT_BossShootBombard.generated.h"
class ABoss;

UCLASS(Blueprintable)
class TWILIGHTARCHERY_API UBTT_BossShootBombard : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere) UClass* BarragePattern = nullptr; 
	ABoss* Boss = nullptr;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere) USoundBase* Sound = nullptr;
};
