#include "ArrowPickup.h"

#include "Components/BoxComponent.h"
#include "TwilightArchery/Player/PlayerCharacter.h"

AArrowPickup::AArrowPickup()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("ArrowBoxCollider");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("ArrowMesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	Tags.Add("ArrowPickup");
}

void AArrowPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AArrowPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->Tags.Contains("Player"))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		Player->PickupArrowsEvent(PickupArrowsCount);
		Player->AddStaminaEvent(StaminaAddedOnPickup);
		NumberOfUses++;

		if (NumberOfUses >= MaxNumberOfUses)
			PickupEmptyDelegate.Broadcast(this);
	}
}

