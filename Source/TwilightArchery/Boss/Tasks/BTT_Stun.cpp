#include "BTT_Stun.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "TwilightArchery/Boss/BossController.h"

EBTNodeResult::Type UBTT_Stun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(BossController))
		BossController = Cast<ABossController>(OwnerComp.GetAIOwner());
	
	if (!IsValid(BossBlackboard) && IsValid(BossController))
		BossBlackboard = BossController->GetBlackboardComponent();

	if (IsValid(BossBlackboard))
	{
		if (BossBlackboard->GetValueAsBool("IsStunned"))
			BossBlackboard->SetValueAsBool("IsStunned",false);
		else
			BossBlackboard->SetValueAsBool("IsStunned",true);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
