#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Kismet/GameplayStatics.h"
#include "BTT_BossShootFromEye.generated.h"
class ABossController;
class ABoss;

UCLASS(Blueprintable)
class TWILIGHTARCHERY_API UBTT_BossShootFromEye : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere) UClass* BarragePattern = nullptr;
	ABossController*        BossController = nullptr;
	ABoss*                  Boss           = nullptr;
	USkeletalMeshComponent* BossHead       = nullptr;

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	UPROPERTY(EditAnywhere) USoundBase* Sound = nullptr;
	
};
