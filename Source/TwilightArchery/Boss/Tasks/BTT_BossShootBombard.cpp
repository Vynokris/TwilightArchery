#include "BTT_BossShootBombard.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TwilightArchery/Boss/Boss.h"
#include "TwilightArchery/Boss/BossController.h"
#include "TwilightArchery/Player/PlayerCharacter.h"

EBTNodeResult::Type UBTT_BossShootBombard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(Boss))
		Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetInstigator());

	if (IsValid(Boss))
	{
		const APlayerCharacter* FocusedPlayer = Boss->GetFocusedPlayer();
		      FVector  SpawnPoint    = FocusedPlayer->GetActorLocation() - FVector(0, 0, 100);
		const FRotator SpawnRotation = FRotator::ZeroRotator;
	
		// Spawn the barrage on the ground by finding its height with a raycast.
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams; CollisionParams.AddIgnoredActor(FocusedPlayer);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, SpawnPoint, SpawnPoint - FVector(0, 0, 1000), ECC_WorldStatic, CollisionParams))
		{
			SpawnPoint.Z = HitResult.Location.Z + 5;
		}
		
		GetWorld()->SpawnActor<AActor>(BarragePattern, SpawnPoint, SpawnRotation);

		if(IsValid(Sound))
		{
			UGameplayStatics::PlaySoundAtLocation(FocusedPlayer,Sound,FocusedPlayer->GetActorLocation(),FocusedPlayer->GetActorRotation(),1.5,1,0,nullptr,nullptr,nullptr);
		}
	}
	return EBTNodeResult::Succeeded;
}
