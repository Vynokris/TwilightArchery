#include "ArrowPickupSpawner.h"

#include "ArrowPickup.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TwilightArchery/Utils/DebugUtils.h"

AArrowPickupSpawner::AArrowPickupSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("ArrowBoxCollider");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = BoxComponent;
}

void AArrowPickupSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Get the spawning area bounds.
	SpawnOrigin = GetActorLocation();
	SpawnExtent = BoxComponent->GetComponentScale() * 32;

	// Setup the respawn cooldown.
	RespawnCooldown.SetDuration(RespawnCooldownDuration);
	RespawnCooldown.CooldownCallback.AddLambda([this]{ SpawnArrowPickup(); });

	// Spawn all the arrow pickups.
	if (ArrowPickupBP)
		for (int i = 0; i < MaxActiveArrowPickups; i++)
			SpawnArrowPickup();
}

void AArrowPickupSpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RespawnCooldown.Tick(DeltaSeconds);
}

void AArrowPickupSpawner::SpawnArrowPickup()
{
	if (ArrowPickups.Num() >= MaxActiveArrowPickups) return;
	
	// Get a random location and rotation for the arrow pickup.
	FVector  RandPoint = UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent); RandPoint.Z = SpawnOrigin.Z + SpawnExtent.Z;
	FRotator Rotation;

	// Place the pickup on the landscape.
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, RandPoint, RandPoint + FVector::DownVector * SpawnExtent.Z * 2, ECC_WorldStatic))
	{
		RandPoint.Z = HitResult.Location.Z;
		RandPoint  += HitResult.Normal * 30;
		Rotation    = HitResult.Normal.Rotation();
	}
	else
	{
		SpawnArrowPickup();
		return;
	}

	// Spawn arrow pickup and add it to the arrow pickups array.
	AArrowPickup* NewArrowPickup = Cast<AArrowPickup>(GetWorld()->SpawnActor(ArrowPickupBP, &RandPoint, &Rotation));
	if (IsValid(NewArrowPickup))
	{
		NewArrowPickup->PickupEmptyDelegate.AddLambda([this](AArrowPickup* ArrowPickup)
		{
			ArrowPickups.Remove(ArrowPickup);
			ArrowPickup->Destroy();
			RespawnCooldown.Stop();
			RespawnCooldown.Start();
		});
		ArrowPickups.Add(NewArrowPickup);
		FVector ActorRot = NewArrowPickup->GetActorRotation().Euler();
		NewArrowPickup->SetActorRelativeRotation(FRotator::MakeFromEuler({ (float)FMath::RandRange(0, 360), ActorRot.Y, ActorRot.Z  } ));
	}
	else
	{
		SpawnArrowPickup();
		return;
	}
}


