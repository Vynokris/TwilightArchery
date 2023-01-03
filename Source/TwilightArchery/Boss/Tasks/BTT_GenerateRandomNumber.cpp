#include "BTT_GenerateRandomNumber.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "TwilightArchery/Boss/BossController.h"


EBTNodeResult::Type UBTT_GenerateRandomNumber::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(BossController))
		BossController = Cast<ABossController>(OwnerComp.GetAIOwner());
	
	if (!IsValid(BossBlackboard) && IsValid(BossController))
		BossBlackboard = BossController->GetBlackboardComponent();

	if (IsValid(BossBlackboard))
	{
		const int RandomNumber = FMath::RandRange(0, RandMax);
		BossBlackboard->SetValueAsInt("RandomNumber", RandomNumber);
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
