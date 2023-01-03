#include "BTT_Rage.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "TwilightArchery/Boss/BossController.h"

EBTNodeResult::Type UBTT_Rage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(BossController))
		BossController = Cast<ABossController>(OwnerComp.GetAIOwner());
	
	if (!IsValid(BossBlackboard) && IsValid(BossController))
		BossBlackboard = BossController->GetBlackboardComponent();

	if (IsValid(BossBlackboard))
	{
		const float Health    = BossBlackboard->GetValueAsFloat("Health");
		const float MaxHealth = BossBlackboard->GetValueAsFloat("MaxHealth");

		if (!WasEnragedBefore && !BossBlackboard->GetValueAsBool("IsEnraged") && Health <= MaxHealth / 2.5)
		{
			BossBlackboard->SetValueAsBool("IsEnraged",true);
			WasEnragedBefore = true;
		}
		else if (WasEnragedBefore)
		{
			BossBlackboard->SetValueAsBool("IsEnraged",false);
		}
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
