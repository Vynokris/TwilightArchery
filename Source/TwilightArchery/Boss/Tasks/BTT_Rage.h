#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Rage.generated.h"
class ABossController;

UCLASS(Blueprintable)

class TWILIGHTARCHERY_API UBTT_Rage : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	ABossController*      BossController   = nullptr;
	UBlackboardComponent* BossBlackboard   = nullptr;
	bool                  WasEnragedBefore = false;

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
