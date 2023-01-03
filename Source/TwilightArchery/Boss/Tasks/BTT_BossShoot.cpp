#include "BTT_BossShoot.h"

#include "AIController.h"
#include "TwilightArchery/Boss/Boss.h"

EBTNodeResult::Type UBTT_BossShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(Boss))
		Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetInstigator());

	if (IsValid(Boss))
	{
		if (HeightAboveGround == 0)
		{
			const FVector BossLocation = Boss->GetActorLocation();
			FHitResult HitResult;
			FCollisionQueryParams CollisionParams; CollisionParams.AddIgnoredActor(Boss);
			if (GetWorld()->LineTraceSingleByChannel(HitResult, BossLocation, BossLocation - FVector(0, 0, 1000), ECC_WorldStatic, CollisionParams))
			{
				HeightAboveGround = BossLocation.Z - HitResult.Location.Z - 35;
			}
		}
		
		const FVector  SpawnPoint    = FVector(Boss->GetActorLocation()) - FVector(0, 0, HeightAboveGround != 0 ? HeightAboveGround : 125);
		const FRotator SpawnRotation = FRotator::ZeroRotator;

		GetWorld()->SpawnActor<AActor>(BarragePattern, SpawnPoint, SpawnRotation);
		if(IsValid(Sound))
		{
			UGameplayStatics::PlaySoundAtLocation(Boss,Sound,Boss->GetActorLocation(),Boss->GetActorRotation(),0.95,1,0,nullptr,nullptr,Boss);
		}

	}

	return EBTNodeResult::Succeeded;
}
