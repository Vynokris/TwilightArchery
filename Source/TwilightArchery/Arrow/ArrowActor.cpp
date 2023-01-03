#include "ArrowActor.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TwilightArchery/Boss/Boss.h"
#include "TwilightArchery/Player/PlayerCharacter.h"
#include "TwilightArchery/UI/GameHUD.h"

#pragma region Setup
AArrowActor::AArrowActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("ArrowBoxCollider");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn,   ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	BoxComponent->SetGenerateOverlapEvents(true);
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("ArrowMesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("TrailParticles");
	ParticleComponent->SetupAttachment(MeshComponent);
	ParticleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ArrowMovement");
	ProjectileMovement->ProjectileGravityScale = 0.8;

	Tags.Add("Arrow");
}

void AArrowActor::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement->Deactivate();
	ParticleComponent->Deactivate();
}
#pragma endregion


#pragma region Update
void AArrowActor::Tick(float DeltaTime)
{
	if (!ProjectileMovement->IsActive()) return;
	
	Super::Tick(DeltaTime);
	const FRotator Rotation = GetActorRotation();
	SetActorRotation({ ProjectileMovement->Velocity.Rotation().Pitch, Rotation.Yaw, Rotation.Roll });
}
#pragma endregion


#pragma region Misc
void AArrowActor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	ParticleComponent->Deactivate();
	
	AttachToActor(Other, FAttachmentTransformRules::KeepWorldTransform);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileMovement->Deactivate();
	SetLifeSpan(DespawnDuration);
	
	if (ABoss* Boss = Cast<ABoss>(Other))
	{
		const bool HitWeakPoint = OtherComp->ComponentHasTag("WeakPoint");
		Boss->SetEnabled(true);
		Boss->TakeDamageEvent(Damage, GetParentPlayer()->GetPlayerIndex(), HitWeakPoint);
		ParentPlayer->GetGameHud()->BossHealthBar->SetVisibility(ESlateVisibility::Visible);
		ParentPlayer->AddScoreEvent(Damage * (HitWeakPoint ? 2 : 1));
		ParentPlayer->AddStaminaEvent(HitWeakPoint ? 4 : 1);
	}
}

void AArrowActor::ActivateTrail() const
{
	ParticleComponent->Activate();
}
#pragma endregion
