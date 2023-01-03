#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Stun.generated.h"
class ABossController;

UCLASS(Blueprintable)
class TWILIGHTARCHERY_API UBTT_Stun : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	ABossController*      BossController = nullptr;
	UBlackboardComponent* BossBlackboard = nullptr;

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
