#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossController.generated.h"
class ABoss;
class UBehaviorTreeComponent;

UCLASS()
class TWILIGHTARCHERY_API ABossController : public AAIController
{
	GENERATED_BODY()

protected:
	ABoss*                  Boss                  = nullptr;
	UBlackboardComponent*   BlackboardComponent   = nullptr;
	UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard Keys") FName IsEnragedKey;
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard Keys") FName IsStunnedKey;
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard Keys") FName HealthKey;
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard Keys") FName MaxHealthKey;
	
	virtual void OnPossess(APawn* PossessedPawn) override;
	virtual void Tick     (float  DeltaTime)     override;

public:
	ABossController();
	
	UFUNCTION(BlueprintCallable) FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
};
