
#include "BossController.h"

#include "Boss.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ABossController::ABossController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BT_Boss");
	BlackboardComponent   = CreateDefaultSubobject<UBlackboardComponent  >("BB_Boss");

	IsEnragedKey = "IsEnraged";
	IsStunnedKey = "IsStunned";
	HealthKey    = "Health";
	MaxHealthKey = "MaxHealth";
}

void ABossController::OnPossess(APawn* PossessedPawn) 
{
	Super::OnPossess(PossessedPawn);

	Boss = Cast<ABoss>(PossessedPawn);

	if (IsValid(Boss) && IsValid(Boss->GetBehaviorTree()))
	{
		BlackboardComponent  ->InitializeBlackboard(*Boss->GetBehaviorTree()->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*Boss->GetBehaviorTree());
		BlackboardComponent  ->SetValueAsFloat(MaxHealthKey, Boss->GetMaxHealth());
	}
}

void ABossController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	BlackboardComponent->SetValueAsFloat(HealthKey, Boss->GetHealth());
	Boss->SetEnraged(BlackboardComponent->GetValueAsBool(IsEnragedKey));
	Boss->SetStunned(BlackboardComponent->GetValueAsBool(IsStunnedKey));
}