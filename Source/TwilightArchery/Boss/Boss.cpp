#include "Boss.h"

#include "BossController.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TwilightArchery/Player/PlayerCharacter.h"
#include "TwilightArchery/UI/GameHUD.h"


#pragma region Setup
ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create behavior tree.
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>("BehaviorTree");

	// Create the boss meshes.
	BossMesh     = CreateDefaultSubobject<USkeletalMeshComponent>("BossMesh");
	BossHeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BossHeadMesh");
	BossHeadMesh->SetupAttachment(BossMesh, "BossHeadMesh");
	RootComponent = BossMesh;

	// Create the weak points colliders.
	EyeWeakPoint  = CreateDefaultSubobject<UBoxComponent>("EyeWeakPoint");
	ArmWeakPoint1 = CreateDefaultSubobject<UBoxComponent>("ArmWeakPoint1");
	ArmWeakPoint2 = CreateDefaultSubobject<UBoxComponent>("ArmWeakPoint2");
	ArmWeakPoint3 = CreateDefaultSubobject<UBoxComponent>("ArmWeakPoint3");
	ArmWeakPoint4 = CreateDefaultSubobject<UBoxComponent>("ArmWeakPoint4");
	EyeWeakPoint ->SetupAttachment(BossHeadMesh, "EyeWeakPoint");
	ArmWeakPoint1->SetupAttachment(BossMesh,     "ArmWeakPoint1");
	ArmWeakPoint2->SetupAttachment(BossMesh,     "ArmWeakPoint2");
	ArmWeakPoint3->SetupAttachment(BossMesh,     "ArmWeakPoint3");
	ArmWeakPoint4->SetupAttachment(BossMesh,     "ArmWeakPoint4");

	// Create the weak points glowing meshes.
	EyeWeakPointGlow  = CreateDefaultSubobject<UStaticMeshComponent>("EyeWeakPointGlow");
	ArmWeakPointGlow1 = CreateDefaultSubobject<UStaticMeshComponent>("ArmWeakPointGlow1");
	ArmWeakPointGlow2 = CreateDefaultSubobject<UStaticMeshComponent>("ArmWeakPointGlow2");
	ArmWeakPointGlow3 = CreateDefaultSubobject<UStaticMeshComponent>("ArmWeakPointGlow3");
	ArmWeakPointGlow4 = CreateDefaultSubobject<UStaticMeshComponent>("ArmWeakPointGlow4");
	EyeWeakPointGlow ->SetupAttachment(BossHeadMesh, "EyeWeakPointGlow");
	ArmWeakPointGlow1->SetupAttachment(BossMesh,     "ArmWeakPointGlow1");
	ArmWeakPointGlow2->SetupAttachment(BossMesh,     "ArmWeakPointGlow2");
	ArmWeakPointGlow3->SetupAttachment(BossMesh,     "ArmWeakPointGlow3");
	ArmWeakPointGlow4->SetupAttachment(BossMesh,     "ArmWeakPointGlow4");
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the HUD.
	Hud = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Initialize health and damage blink cooldown.
	Health = MaxHealth;
	DamageBlinkCooldown.SetDuration(DamageBlinkDuration);
	DamageBlinkCooldown.CooldownCallback.AddLambda([this]
	{
		if      (IsEnraged()) SetWeakPointsMaterial(WeakPointEnragedMaterial);
		else if (IsStunned()) SetWeakPointsMaterial(WeakPointStunnedMaterial);
		else                  SetWeakPointsMaterial(WeakPointBasicMaterial);
	});

	// Add the boss tag.
	Tags.Add("Boss");
}
#pragma endregion

#pragma region Utility
bool ABoss::AreWeakPointsValid() const
{
	return IsValid(EyeWeakPointGlow)  &&
		   IsValid(ArmWeakPointGlow1) &&
		   IsValid(ArmWeakPointGlow2) &&
		   IsValid(ArmWeakPointGlow3) &&
		   IsValid(ArmWeakPointGlow4);
}

void ABoss::SetWeakPointsMaterial(UMaterialInstance* Material) const
{
	if (!AreWeakPointsValid() || !IsValid(Material)) return;
	EyeWeakPointGlow ->SetMaterial(0, Material);
	ArmWeakPointGlow1->SetMaterial(0, Material);
	ArmWeakPointGlow2->SetMaterial(0, Material);
	ArmWeakPointGlow3->SetMaterial(0, Material);
	ArmWeakPointGlow4->SetMaterial(0, Material);
}
#pragma endregion 


#pragma region Update
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DamageBlinkCooldown.Tick(DeltaTime);

	// Focus the player that has done the most damage.
	if (IsValid(Players[0]) && IsValid(Players[1]))
	{
		FocusedPlayer = DamageDoneByPlayer[0] >= DamageDoneByPlayer[1] ? Players[0] : Players[1];
		if      (Players[0]->IsDead()) FocusedPlayer = Players[1];
		else if (Players[1]->IsDead()) FocusedPlayer = Players[0];
	}

	// Slowly rotate the boss.
	if (!DamageBlinkCooldown.HasStarted())
	{
		SetActorRotation(GetActorRotation() + FRotator(0, RotationDir * RotationSpeed * DeltaTime, 0));
		if (FMath::RandRange(0, 999) == 0) RotationDir *= -1;
	}

	// Rotate boss head towards the focused player.
	if (IsValid(FocusedPlayer))
	{
		const FRotator BossToPlayerRot = (FocusedPlayer->GetActorLocation() - BossHeadMesh->GetComponentLocation()).Rotation();
		const FRotator HeadRotation    = { 0, BossToPlayerRot.Yaw - 90, FMath::Clamp(-BossToPlayerRot.Pitch, -90.f, 10.f) };
		BossHeadMesh->SetWorldRotation(HeadRotation);
	}
}
#pragma endregion 


#pragma region Events
void ABoss::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Launch player away if he hits the boss.
	if (Other->Tags.Contains("Player"))
	{
		FVector ExpulsionVector = Other->GetActorLocation() - GetActorLocation();
		ExpulsionVector = { ExpulsionVector.X, ExpulsionVector.Y, 0 };
		ExpulsionVector.Normalize();
		ExpulsionVector.Z = 1;
		Cast<ACharacter>(Other)->GetCharacterMovement()->Launch(ExpulsionVector * 1000);
	}
}

void ABoss::SetEnabled(const bool& Value)
{
	if (Enabled == Value) return;
	Enabled = Value;
	
	// Get both players if it has not yet been done.
	if (Value && (!IsValid(Players[0]) || !IsValid(Players[1])))
		GetPlayers();
	
	// Set the blackboard IsEnabled value.
	if (const ABossController* BossController = Cast<ABossController>(GetController()))
	{
		BossController->GetBlackboardComponent()->SetValueAsBool("IsEnabled", Value);
	}
}

void ABoss::SetEnraged(const bool& Value)
{
	if (Enraged == Value) return;
	
	Enraged        = Value;
	RotationSpeed *= (Enraged ? 10 : 0.1);
	if      ( Enraged) SetWeakPointsMaterial(WeakPointEnragedMaterial);
	else if (!Stunned) SetWeakPointsMaterial(WeakPointBasicMaterial  );
}

void ABoss::SetStunned(const bool& Value)
{
	if (Stunned == Value) return;
	
	Stunned       = Value;
	RotationSpeed = (Stunned ? 0 : 4);
	if      ( Stunned) SetWeakPointsMaterial(WeakPointStunnedMaterial);
	else if (!Enraged) SetWeakPointsMaterial(WeakPointBasicMaterial);
}

void ABoss::TakeDamageEvent(const float& Damage, const int& AttackingPlayerId, const bool& HitWeakPoint)
{
	const float AppliedDamage = Damage * (HitWeakPoint ? 2 : 1);
	Health -= AppliedDamage;
	DamageDoneByPlayer[AttackingPlayerId] += AppliedDamage;
	Hud->BossHealthBar->UpdateWidget(Health, MaxHealth);
	
	if (Health <= 0)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, this->GetActorLocation(), this->GetActorRotation(), 2, 1, 0, nullptr, nullptr, this);
		DeathEvent();
		return;
	}
	else if(HitWeakPoint)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeakPointSound, this->GetActorLocation(), this->GetActorRotation(), 1, 1, 0, nullptr, nullptr, this);
	}
	else
	{
		const int rand = FMath::RandRange(0, 2);
		UGameplayStatics::PlaySoundAtLocation(this, HitBodySound[rand], this->GetActorLocation(), this->GetActorRotation(), 1, 1, 0, nullptr, nullptr, this);
	}

	if (HitWeakPoint && !IsStunned())
	{
		DamageBlinkCooldown.Start();
		SetWeakPointsMaterial(IsEnraged() ? WeakPointBlinkEnragedMaterial : WeakPointBlinkBasicMaterial);
	}
}

void ABoss::DeathEvent()
{
	Destroy();
	if (IsValid(Hud))
		Hud->VictoryMenu->ShowMenu();

	Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn())->InvincibilityEvent(69);
	Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetPawn())->InvincibilityEvent(69);
}

void ABoss::GetPlayers()
{
	// Get the players.
	Players[0] = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	Players[1] = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetCharacter());

	// Set the default focused player.
	FocusedPlayer = Players[FMath::RandRange(0, 1)];
}
#pragma endregion 
