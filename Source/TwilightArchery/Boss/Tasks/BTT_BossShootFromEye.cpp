#include "BTT_BossShootFromEye.h"

#include "GameFramework/Character.h"
#include "TwilightArchery/Boss/Boss.h"
#include "TwilightArchery/Boss/BossController.h"
#include "TwilightArchery/Player/PlayerCharacter.h"

EBTNodeResult::Type UBTT_BossShootFromEye::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(BossController))
		BossController = Cast<ABossController>(OwnerComp.GetAIOwner());
	
	if (!IsValid(Boss) && IsValid(BossController))
		Boss = Cast<ABoss>(BossController->GetInstigator());
	
	if (!IsValid(BossHead) && IsValid(Boss))
		BossHead = Cast<USkeletalMeshComponent>(Boss->GetDefaultSubobjectByName("BossHeadMesh"));

	if (IsValid(BossController) && IsValid(BossHead))
	{
		const FVector  SpawnPoint    = FVector(BossHead->GetComponentLocation());
		const FRotator SpawnRotation = (Boss->GetFocusedPlayer()->GetActorLocation() - SpawnPoint).Rotation();
		GetWorld()->SpawnActor<AActor>(BarragePattern, SpawnPoint, SpawnRotation);

		if(IsValid(Sound))
			UGameplayStatics::PlaySoundAtLocation(Boss,Sound,Boss->GetActorLocation(),Boss->GetActorRotation(),0.75,1,0,nullptr,nullptr,Boss);
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
