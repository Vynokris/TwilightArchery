#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TwilightArchery/UI/GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "TwilightArchery/Arrow/ArrowActor.h"
#include "TwilightArchery/UI/Widgets/BowCrosshair.h"
#include "TwilightArchery/Utils/DebugUtils.h"


#pragma region Setup
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
    bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;
	
	// Setup camera spring arm.
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	SpringArmComponent->bEnableCameraLag        = true;
	SpringArmComponent->CameraLagSpeed          = CameraLag;
	SpringArmComponent->TargetArmLength         = CameraDistance;
    SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetRelativeLocation(CameraOffset);

	// Setup camera.
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;

	// Setup character movement.
	CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->bUseControllerDesiredRotation = false;
	CharacterMovementComponent->bOrientRotationToMovement     = true;
	CharacterMovementComponent->RotationRate                  = FRotator(0, 600, 0);
	CharacterMovementComponent->JumpZVelocity                 = 600;
	CharacterMovementComponent->AirControl                    = 1;

	// Ignore collision with other players.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	// Add the player tag.
	Tags.Add("Player");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    
	// Update camera parameters.
	SpringArmComponent->SetRelativeLocation(CameraOffset);
	SpringArmComponent->CameraLagSpeed  = CameraLag;
	SpringArmComponent->TargetArmLength = CameraDistance;
	CameraComponent   ->FieldOfView     = DefaultFov;
	WalkingSpeed = CharacterMovementComponent->MaxWalkSpeed;
	GravityScale = CharacterMovementComponent->GravityScale;

	// Set the player's index.
	PlayerIndex = 1 - static_cast<int>(GetWorld()->GetFirstPlayerController() == Cast<APlayerController>(GetController()));
	
	// Get the HUD.
	GameHud = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Set all cooldown durations.
	StaminaReloadCooldown.SetDuration(StaminaRefillCooldownDuration);
	DashTimer            .SetDuration(DashingDuration);
	DashCooldown         .SetDuration(DashCooldownDuration);
	SuperjumpTimer       .SetDuration(SuperjumpDuration);
	SuperjumpCooldown    .SetDuration(SuperjumpCooldownDuration);
	FovTransitionCooldown.SetDuration(FovTransitionDuration);
	ReanimationTimer     .SetDuration(ReanimationDuration);

	// Set cooldown callbacks.
	DashTimer        .CooldownCallback.AddLambda([this]{ StopDashingEvent();   });
	DashCooldown     .CooldownCallback.AddLambda([this]{ GameHud->CooldownWidget->SetVisibilityCooldownOff(PlayerIndex>0,true,false);   });
	SuperjumpTimer   .CooldownCallback.AddLambda([this]{ StopSuperjumpEvent(); });
	SuperjumpCooldown.CooldownCallback.AddLambda([this] {GameHud->CooldownWidget->SetVisibilityCooldownOff(PlayerIndex>0,false,true);   });
	ReanimationTimer .CooldownCallback.AddLambda([this]{ StopReanimateEvent(); });

	// Initialize health, stamina and arrows.
	Health     = MaxHealth;
	Stamina    = MaxStamina;
	ArrowCount = MaxArrowCount;
	LockRotationToCam(true);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Action inputs.
	PlayerInputComponent->BindAction("Jump",      IE_Pressed,  this, &APlayerCharacter::JumpEvent);
	PlayerInputComponent->BindAction("Jump",      IE_Released, this, &ACharacter      ::StopJumping);
	PlayerInputComponent->BindAction("Superjump", IE_Pressed,  this, &APlayerCharacter::StartSuperjumpEvent);
	PlayerInputComponent->BindAction("Run",       IE_Pressed,  this, &APlayerCharacter::StartRunningEvent);
	PlayerInputComponent->BindAction("Run",       IE_Released, this, &APlayerCharacter::StopRunningEvent);
	PlayerInputComponent->BindAction("Dash",      IE_Pressed,  this, &APlayerCharacter::StartDashingEvent);
	PlayerInputComponent->BindAction("Reanimate", IE_Pressed,  this, &APlayerCharacter::StartReanimateEvent);
	PlayerInputComponent->BindAction("Reanimate", IE_Released, this, &APlayerCharacter::StopReanimateEvent);

	// Player movement input.
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",   this, &APlayerCharacter::MoveRight);

	// Camera movement input.
	PlayerInputComponent->BindAxis("LookRightMouse",   this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpMouse",      this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRightGamepad", this, &APlayerCharacter::LookRightAtRate);
	PlayerInputComponent->BindAxis("LookUpGamepad",    this, &APlayerCharacter::LookUpAtRate);

	// Bow actions input.
	PlayerInputComponent->BindAction("PullBow",  IE_Pressed,   this, &APlayerCharacter::StartAimingEvent);
	PlayerInputComponent->BindAction("PullBow",  IE_Released,  this, &APlayerCharacter::StopAimingEvent);
	PlayerInputComponent->BindAction("ShootBow", IE_Pressed,   this, &APlayerCharacter::ShootEvent);

	// Pause input.
	FInputActionBinding PauseAB("Pause", IE_Pressed);
	PauseAB.ActionDelegate.GetDelegateForManualSet().BindLambda([this]{ if (GameHud && GameHud->PauseMenu) GameHud->PauseMenu->Pause(); });
	InputComponent->AddActionBinding(PauseAB);
}
#pragma endregion


#pragma region Camera
void APlayerCharacter::LockRotationToCam(const bool& Locked) const
{
	if (Locked)
	{
		CharacterMovementComponent->bUseControllerDesiredRotation = true;
		CharacterMovementComponent->bOrientRotationToMovement = false;
	}
	else
	{
		CharacterMovementComponent->bUseControllerDesiredRotation = false;
		CharacterMovementComponent->bOrientRotationToMovement = true;
	}
}

void APlayerCharacter::LookRightAtRate(float Rate)
{
	if (!AimingTimer.HasStarted())
		// Calculate delta for this frame from the rate information.
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	else 
		AddControllerYawInput(Rate * AimingTurnRate * GetWorld()->GetDeltaSeconds());

}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	if (!AimingTimer.HasStarted())
		// Calculate delta for this frame from the rate information.
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	else
		AddControllerPitchInput(Rate * AimingLookUpRate * GetWorld()->GetDeltaSeconds());
}
#pragma endregion


#pragma region Movement Events
void APlayerCharacter::MoveForward(float Amount)
{
	if (IsDead() || IsReanimating() || Amount == 0) return;
	if (!IsRunning() && CanRun() && RunInputDown) StartRunningEvent();

	// get forward vector
	const FVector Direction = FRotationMatrix({ 0, Controller->GetControlRotation().Yaw, 0 }).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Amount);


}

void APlayerCharacter::MoveRight(float Amount)
{
	if (IsDead() || IsReanimating() || Amount == 0) return;
	if (!IsRunning() && CanRun() && RunInputDown) StartRunningEvent();
	
	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Amount);
}

void APlayerCharacter::JumpEvent()
{
	if(GetCharacterMovement()->Velocity.Z==0 && IsValid(JumpSound))
		UGameplayStatics::PlaySoundAtLocation(this,JumpSound,this->GetActorLocation(),this->GetActorRotation(),3,1,0,nullptr,nullptr,this);

	if (!IsDead() && !IsReanimating()) Super::Jump();
}

void APlayerCharacter::StartRunningEvent()
{
	RunInputDown = true;
	if (IsDead() || !CanRun()) return;
	
	LockRotationToCam(false);

	CharacterMovementComponent->MaxWalkSpeed = RunningSpeed;
	StartFovTransition(RunningFov, FovTransitionDuration);
	Running = true;
}

void APlayerCharacter::StopRunningEvent()
{
	RunInputDown = false;
	if (!IsRunning()) return;
	
	CharacterMovementComponent->MaxWalkSpeed = WalkingSpeed;
	StartFovTransition(DefaultFov, FovTransitionDuration);
	LockRotationToCam(true);
	Running = false;
}

void APlayerCharacter::StartDashingEvent()
{

	if (IsDead() || !CanDash()) return;
	if (IsRunning()) StopRunningEvent();
	if (IsAiming ()) StopAimingEvent ();
	
	if(IsValid(DashSound))
		UGameplayStatics::PlaySoundAtLocation(this,DashSound,this->GetActorLocation(),this->GetActorRotation(),5,1,0,nullptr,nullptr,this);

	StartFovTransition(DashingFov, DashingDuration);
	UseStaminaEvent   (DashingStaminaCost);
	InvincibilityEvent(DashingDuration);
	CharacterMovementComponent->GravityScale = 0;
	CharacterMovementComponent->Velocity.Z   = 0;
	DashTimer.Start();
	GameHud->CooldownWidget->UpdateWidget(DashCooldownDuration, DashCooldownDuration, PlayerIndex > 0, true, false);
	GameHud->CooldownWidget->SetVisibilityCooldownOn(PlayerIndex > 0, true, false);
}

void APlayerCharacter::StopDashingEvent()
{
	StartFovTransition(DefaultFov, FovTransitionDuration);
	CharacterMovementComponent->GravityScale = GravityScale;
	DashCooldown.Start();
}

void APlayerCharacter::StartSuperjumpEvent()
{

	if (IsDead() || !CanSuperjump()) return;
	if (IsRunning()) StopRunningEvent();
	if (IsAiming ()) StopAimingEvent ();
	
	UseStaminaEvent   (SuperjumpStaminaCost);
	InvincibilityEvent(SuperjumpDuration);
	SetActorLocation(GetActorLocation() + FVector::UpVector * 200);
	CharacterMovementComponent->GravityScale = 0;
	CharacterMovementComponent->Velocity.Z   = 0;
	SuperjumpTimer.Start();
	GameHud->CooldownWidget->UpdateWidget(SuperjumpCooldownDuration, SuperjumpCooldownDuration, PlayerIndex > 0, false, true);
	GameHud->CooldownWidget->SetVisibilityCooldownOn(PlayerIndex > 0, false, true);

	if(IsValid(SuperJumpSound))
		UGameplayStatics::PlaySoundAtLocation(this,SuperJumpSound,this->GetActorLocation(),this->GetActorRotation(),1,1,0,nullptr,nullptr,this);
}

void APlayerCharacter::StopSuperjumpEvent()
{
	CharacterMovementComponent->GravityScale = GravityScale;
	SuperjumpCooldown.Start();
}
#pragma endregion


#pragma region Bow Events
void APlayerCharacter::StartAimingEvent()
{
	if (IsDead() || IsReanimating() || GetArrowCount() <= 0) return;
	
	// Start pulling timer and transition to aiming FOV.
	StartFovTransition(AimingFov, MaxAimingDuration * (1 - GetAimingCompletion()));
	CharacterMovementComponent->MaxWalkSpeed = WalkingSpeed * AimingWalkSpeedMultiplier;
	GameHud->BowCrosshairWidget->UpdateWidget(this);
	LockRotationToCam(true);
	AimingTimer.Start();
	AimingCancelled = false;

	if(IsValid(AimSound))
		UGameplayStatics::PlaySoundAtLocation(this,AimSound,this->GetActorLocation(),this->GetActorRotation(),0.80,1,0,nullptr,nullptr,this);
}

void APlayerCharacter::StopAimingEvent()
{
	if (!AimingTimer.HasStarted()) return;

	// Stop pulling on the bow.
	AimingCancelled = true;
	if (AimingTimer.GetCurrentTime() > MaxAimingDuration)
		AimingTimer.SetCurrentTime(MaxAimingDuration);

	// Transition back to default FOV.
	StartFovTransition(DefaultFov, MaxAimingDuration * GetAimingCompletion());
	CharacterMovementComponent->MaxWalkSpeed = WalkingSpeed;
	LockRotationToCam(true);
}

void APlayerCharacter::ShootEvent()
{
	if (IsDead() || IsReanimating() || AimingTimer.GetCurrentTime() <= 0 || GetArrowCount() <= 0 || AimingCancelled) return;
	
	// Transition back to default FOV.
	StartFovTransition(DefaultFov, 0.1f);
	CharacterMovementComponent->MaxWalkSpeed = WalkingSpeed;

	// Spawn a new arrow.
	const FVector  Location = GetActorLocation();
	const FRotator Rotation = GetActorRotation();
	AArrowActor* Arrow = Cast<AArrowActor>(GetWorld()->SpawnActor(ArrowActorBP, &Location, &Rotation));
	Arrow->SetActorLocation(GetMesh()->GetSocketLocation("ArrowSocket"));

	// Compute the shooting force and give velocity to the current arrow.
	const float ShootingForce = FMath::Lerp(ShootingMinForce, ShootingMaxForce, GetAimingCompletion());
	Arrow->GetProjectileMovement()->Activate();
	Arrow->GetProjectileMovement()->MaxSpeed = ShootingMaxForce;
	Arrow->GetProjectileMovement()->Velocity = GetCamera()->GetComponentRotation().RotateVector(FVector::ForwardVector) * ShootingForce;
	Arrow->SetDamage(GetAimingCompletion() * MaxDamage);
	Arrow->SetParentPlayer(this);
	Arrow->ActivateTrail();

	// Stop pulling on the bow and reset its scale.
	ArrowCount--;
	ShootFramesCounter = 2;
	AimingTimer.Reset();
	GameHud->BowCrosshairWidget->UpdateWidget(this);
	LockRotationToCam(true);

	if(IsValid(ShootSound))
		UGameplayStatics::PlaySoundAtLocation(this,ShootSound,this->GetActorLocation(),this->GetActorRotation(),1,1,0,nullptr,nullptr,this);
}


void APlayerCharacter::PickupArrowsEvent(const int& Amount)
{
	if (IsDead()) return;
	
	ArrowCount = FMath::Clamp(ArrowCount + Amount, 0, MaxArrowCount);
	GameHud->BowCrosshairWidget->UpdateWidget(this);
}
#pragma endregion


#pragma region Misc Events
void APlayerCharacter::TakeDamageEvent(const float& Amount)
{
	if (IsInvincible()) return;
	
	Health -= Amount;
	if (Health <= 0)
	{
		if(Health==0)
			UGameplayStatics::PlaySoundAtLocation(this,DeathSound,this->GetActorLocation(),this->GetActorRotation(),1,1,0,nullptr,nullptr,this);
		DeathEvent();
	}
	else
	{
		const int rand = FMath::RandRange(0, 3);
		UGameplayStatics::PlaySoundAtLocation(this,GruntSound[rand],this->GetActorLocation(),this->GetActorRotation(),2,1,0,nullptr,nullptr,this);
	}
	InvincibilityEvent(InvincibilityDuration);
	GameHud->PlayerHealthBar->UpdateWidget(Health, MaxHealth, PlayerIndex >= 1);
}

void APlayerCharacter::HealEvent(const float& Amount)
{
	if (Health <= 0) LockRotationToCam(true);
	
	Health += Amount;
	if (Health > MaxHealth)
		Health = MaxHealth;
	GameHud->PlayerHealthBar->UpdateWidget(Health, MaxHealth, PlayerIndex >= 1);
}

void APlayerCharacter::InvincibilityEvent(const float& Duration)
{
	InvincibilityTimer.SetDuration(Duration);
	InvincibilityTimer.Start();
}

void APlayerCharacter::UseStaminaEvent(const float& Amount)
{
	Stamina -= Amount;
	if (Stamina < 0)
		Stamina = 0;
	else
		StaminaReloadCooldown.Start();
	GameHud->PlayerStaminaBar->UpdateWidget(Stamina, MaxStamina, PlayerIndex >= 1);
}

void APlayerCharacter::AddStaminaEvent(const float& Amount)
{
	Stamina += Amount;
	if (Stamina > MaxStamina)
		Stamina = MaxStamina;
	GameHud->PlayerStaminaBar->UpdateWidget(Stamina, MaxStamina, PlayerIndex >= 1);
}

void APlayerCharacter::AddScoreEvent(const float& Amount)
{
	Score += Amount;
	GameHud->PlayerScoreWidget->UpdateWidget(Score, PlayerIndex >= 1);
}

void APlayerCharacter::DeathEvent()
{
	if (Health > 0) return;
	
	Health = 0;
	LockRotationToCam(false);
	if (IsAiming ()) StopAimingEvent ();
	if (IsRunning()) StopRunningEvent();
	if (IsDashing()) StopDashingEvent();
	if (IsOtherPlayerDead())
		GameHud->GameOverMenu->ShowMenu();
}

void APlayerCharacter::StartReanimateEvent()
{
	if (!IsValid(GetOtherPlayer()) || !OtherPlayer->IsDead()) return;

	if ((GetActorLocation() - OtherPlayer->GetActorLocation()).Size2D() <= ReanimationDistance)
	{
		ReanimationTimer.Start();
		GameHud->ReviveBar->SetVisibility(ESlateVisibility::Visible);
		
		if (IsRunning()) StopRunningEvent();
		if (IsDashing()) StopDashingEvent();
		if (IsAiming ()) StopAimingEvent ();
	}
}

void APlayerCharacter::StopReanimateEvent()
{
	if (!IsValid(GetOtherPlayer()) || !OtherPlayer->IsDead()) return;
	
	if (ReanimationTimer.HasStarted())
	{
		ReanimationTimer.Stop(false);
		GameHud->ReviveBar->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	if ((GetActorLocation() - OtherPlayer->GetActorLocation()).Size2D() <= ReanimationDistance)
	{
		GameHud->ReviveBar->SetVisibility(ESlateVisibility::Hidden);
		OtherPlayer->HealEvent(5);
	}
}
#pragma endregion


#pragma region Utilities
void APlayerCharacter::StartFovTransition(const float& TargetFov, const float& Duration)
{
	FovTransitionCooldown.Stop(false);
	FovTransitionStart  = CameraComponent->FieldOfView;
	FovTransitionTarget = TargetFov;
	FovTransitionCooldown.SetDuration(Duration);
	FovTransitionCooldown.Start();
}

bool APlayerCharacter::CanRun() const
{
	return !IsRunning()     &&
		   !IsAiming()      &&
		   !IsReanimating() &&
	        Stamina > 0.1   &&
		    CharacterMovementComponent->Velocity.SizeSquared2D() > 25;
}

bool APlayerCharacter::CanDash() const
{
	return !IsDashing()                  &&
		   !IsSuperjumping()             &&
		   !IsReanimating()              &&
		   !DashCooldown.HasStarted()    &&
		   	Stamina > DashingStaminaCost &&
		   	CharacterMovementComponent->Velocity.SizeSquared2D() > 25;
}

bool APlayerCharacter::CanSuperjump() const
{
	return !IsSuperjumping()                        &&
		   !IsDashing()                             &&
		   !IsReanimating()                         &&
		   !CharacterMovementComponent->IsFalling() &&
		   !SuperjumpCooldown.HasStarted()          &&
		   	Stamina > SuperjumpStaminaCost;
}

bool APlayerCharacter::IsOtherPlayerDead()
{
	if (IsValid(GetOtherPlayer()))
		return OtherPlayer->IsDead();
	return false;
}

APlayerCharacter* APlayerCharacter::GetOtherPlayer()
{
	if (!IsValid(OtherPlayer))
	{
		// Get the other player.
		FConstPlayerControllerIterator PlayerIterator = GetWorld()->GetPlayerControllerIterator();
		if (PlayerIndex == 0) PlayerIterator += 1;
		OtherPlayer = Cast<APlayerCharacter>(PlayerIterator->Get()->GetPawn());
		if (!IsValid(OtherPlayer)) return nullptr;
	}
	return OtherPlayer;
}
#pragma endregion


#pragma region Update
void APlayerCharacter::UpdateTimers(const float& DeltaTime)
{
	FovTransitionCooldown.Tick(DeltaTime);
	StaminaReloadCooldown.Tick(DeltaTime);
	InvincibilityTimer   .Tick(DeltaTime);
	DashTimer            .Tick(DeltaTime);
	DashCooldown         .Tick(DeltaTime);
	SuperjumpTimer       .Tick(DeltaTime);
	SuperjumpCooldown    .Tick(DeltaTime);
	ReanimationTimer     .Tick(DeltaTime);
	AimingTimer          .Tick(AimingCancelled ? -DeltaTime : DeltaTime);
}

void APlayerCharacter::UpdateStamina(const float& DeltaTime)
{
	// Deplete stamina while running or aiming.
	if (IsRunning()) UseStaminaEvent(RunningStaminaCost * DeltaTime);
	if (IsAiming ()) UseStaminaEvent(AimingStaminaCost  * DeltaTime);

	// Refill stamina once the cooldown is done.
	if (Stamina < MaxStamina && !IsRunning() && !IsDashing() && !StaminaReloadCooldown.HasStarted()) 
	{
		Stamina += StaminaRefillSpeed * DeltaTime;
		GameHud->PlayerStaminaBar->UpdateWidget(Stamina, MaxStamina, PlayerIndex >= 1);
	}
}

void APlayerCharacter::UpdateFov() const
{
	if (FovTransitionCooldown.HasStarted())
		CameraComponent->FieldOfView = FMath::Lerp(FovTransitionStart, FovTransitionTarget, FovTransitionCooldown.GetCompletionPercent());
}

void APlayerCharacter::UpdateActions(const float& DeltaTime)
{
	// Running.
	if (IsRunning() && (CharacterMovementComponent->Velocity.SizeSquared2D() < 25 || Stamina <= 0)) {
		StopRunningEvent();
		RunInputDown = true;
	}

	// Dashing.
	if (IsDashing()) {
		FVector DashingDir = { CharacterMovementComponent->Velocity.X, CharacterMovementComponent->Velocity.Y, 0 }; DashingDir.Normalize();
		SetActorLocation(GetActorLocation() + DashingDir * DashingVelocity * DeltaTime);
	}

	// Superjumping.
	if (IsSuperjumping()) {
		SetActorLocation(GetActorLocation() + FVector::UpVector * SuperjumpVelocity * FMath::Clamp(1 - SuperjumpTimer.GetCompletionPercent() * 1.12f, 0.f, 1.f) * DeltaTime);
	}

	// Aiming.
	if (IsAiming() && Stamina <= 0) {
		StopAimingEvent();
	}
	if (AimingTimer.HasStarted()) {
		GameHud->BowCrosshairWidget->UpdateWidget(this);
	}

	// Shooting.
	if (ShootFramesCounter > 0) {
		ShootFramesCounter--;
	}

	// Reanimation Widget.
	if (IsReanimating()) {
		GameHud->ReviveBar->UpdateWidget(ReanimationTimer.GetCurrentTime(),ReanimationTimer.GetCooldownDuration());
	}
	
	// Dashing Widget.
	if(DashCooldown.HasStarted()){
		GameHud->CooldownWidget->UpdateWidget(DashCooldown.GetCurrentTime(),DashCooldown.GetCooldownDuration(),PlayerIndex>0,true,false);
	}

	// Superjumping Widget.
	if(SuperjumpCooldown.HasStarted()){
		GameHud->CooldownWidget->UpdateWidget(SuperjumpCooldown.GetCurrentTime(),SuperjumpCooldown.GetCooldownDuration(),PlayerIndex>0,false,true);
	}
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTimers (DeltaTime);
	UpdateStamina(DeltaTime);
	UpdateFov    ();
	UpdateActions(DeltaTime);
}
#pragma endregion
