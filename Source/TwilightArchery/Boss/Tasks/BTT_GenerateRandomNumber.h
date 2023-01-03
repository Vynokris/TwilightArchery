#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GenerateRandomNumber.generated.h"
class ABossController;

UCLASS(Blueprintable)
class TWILIGHTARCHERY_API UBTT_GenerateRandomNumber : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere) int RandMax = 1;
	ABossController*      BossController = nullptr;
	UBlackboardComponent* BossBlackboard = nullptr;

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
