#pragma once

#include <ThirdParty/PhysX3/PxShared/src/foundation/include/PsArray.h>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TwilightArchery/Utils/Cooldown.h"
#include "TwilightArchery/Utils/TimerExt.h"
#include "PlayerCharacter.generated.h"
class AArrowActor;
class UBowCrosshair;
class USpringArmComponent;
class UCameraComponent;
class AGameHUD;


UCLASS(Blueprintable)
class TWILIGHTARCHERY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Components") USpringArmComponent*         SpringArmComponent         = nullptr;
	UPROPERTY(EditAnywhere, Category = "Components") UCameraComponent*            CameraComponent            = nullptr;
    UPROPERTY(EditAnywhere, Category = "Components") UCharacterMovementComponent* CharacterMovementComponent = nullptr;
    
	UPROPERTY(EditAnywhere, Category = "Camera") float   CameraLag      = 15;
	UPROPERTY(EditAnywhere, Category = "Camera") float   CameraDistance = 250;
	UPROPERTY(EditAnywhere, Category = "Camera") FVector CameraOffset   = { 0, 50, 90 };
	UPROPERTY(EditAnywhere, Category = "Camera") float   BaseTurnRate   = 75;
	UPROPERTY(EditAnywhere, Category = "Camera") float   BaseLookUpRate = 75;
	UPROPERTY(EditAnywhere, Category = "Camera") float   AimingLookUpRate = 30;
	UPROPERTY(EditAnywhere, Category = "Camera") float   AimingTurnRate = 30;

	UPROPERTY(EditAnywhere, Category = "Player Properties") int   MaxHealth                     = 5;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float MaxStamina                    = 30;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float StaminaRefillSpeed            = 4;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float StaminaRefillCooldownDuration = 1.5f;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float InvincibilityDuration         = 1.5f;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float ReanimationDistance           = 200;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float ReanimationDuration           = 1.5;
	
	UPROPERTY(EditAnywhere, Category = "Bow Properties") TSubclassOf<AArrowActor> ArrowActorBP = nullptr;
	UPROPERTY(EditAnywhere, Category = "Bow Properties") int   MaxArrowCount             = 15;
	UPROPERTY(EditAnywhere, Category = "Bow Properties") float MaxAimingDuration         = 2;
	UPROPERTY(EditAnywhere, Category = "Bow Properties") float MaxDamage                 = 2;
	UPROPERTY(EditAnywhere, Category = "Bow Properties") float AimingStaminaCost         = 1.5;
	UPROPERTY(EditAnywhere, Category = "Bow Properties") float AimingWalkSpeedMultiplier = 0.7;
	UPROPERTY(EditAnywhere, Category = "Bow Properties") float ShootingMinForce          = 1000;
	UPROPERTY(EditAnywhere, Category = "Bow Properties") float ShootingMaxForce          = 5000;

	UPROPERTY(EditAnywhere, Category = "Dynamic FOV") float FovTransitionDuration = 0.3f;
	UPROPERTY(EditAnywhere, Category = "Dynamic FOV") float DefaultFov            = 90;
	UPROPERTY(EditAnywhere, Category = "Dynamic FOV") float RunningFov            = 100;
	UPROPERTY(EditAnywhere, Category = "Dynamic FOV") float DashingFov            = 100;
	UPROPERTY(EditAnywhere, Category = "Dynamic FOV") float AimingFov             = 60;
	
	UPROPERTY(EditAnywhere, Category = "Character Movement: Running") float RunningSpeed       = 800;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Running") float RunningStaminaCost = 1;
	
	UPROPERTY(EditAnywhere, Category = "Character Movement: Dashing") float DashingVelocity      = 2000;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Dashing") float DashingDuration      = 0.3f;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Dashing") float DashingStaminaCost   = 8;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Dashing") float DashCooldownDuration = 1;
	
	UPROPERTY(EditAnywhere, Category = "Character Movement: Superjump") float SuperjumpVelocity         = 4000;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Superjump") float SuperjumpDuration         = 0.7f;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Superjump") float SuperjumpStaminaCost      = 15;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Superjump") float SuperjumpCooldownDuration = 5;

	UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* DeathSound     = nullptr;
	UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* GruntSound[4]  = { nullptr, nullptr, nullptr, nullptr };
	UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* AimSound       = nullptr;
	UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* ShootSound     = nullptr;
	UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* DashSound      = nullptr;
	UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* JumpSound      = nullptr;
	UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* SuperJumpSound = nullptr;
	

	int   PlayerIndex   = 0;
	float Health        = 0;
	float Stamina       = 0;
	int   Score         = 0;
	int   ArrowCount    = 0;
	float WalkingSpeed  = 0;
	float GravityScale  = 0;

	bool RunInputDown       = false;
	bool Running            = false;
	bool AimingCancelled    = false;
	int  ShootFramesCounter = 0;
	
	FCooldown DashTimer             = { 0 };
	FCooldown DashCooldown          = { 0 };
	FCooldown SuperjumpTimer        = { 0 };
	FCooldown SuperjumpCooldown     = { 0 };
	FCooldown StaminaReloadCooldown = { 0 };
	FCooldown InvincibilityTimer    = { 0 };
	FCooldown ReanimationTimer      = { 0 };
	FTimerExt AimingTimer           = {};
	
	FCooldown FovTransitionCooldown = { 0 };
	float     FovTransitionStart  = -1;
	float     FovTransitionTarget = -1;

	APlayerCharacter* OtherPlayer     = nullptr;
	AGameHUD*         GameHud         = nullptr;

protected:
	void MoveForward    (float Amount);
	void MoveRight      (float Amount);
	void LookRightAtRate(float Rate);
	void LookUpAtRate   (float Rate);
	
	void UpdateTimers (const float& DeltaTime);
	void UpdateStamina(const float& DeltaTime);
	void UpdateFov    () const;
	void UpdateActions(const float& DeltaTime);
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

public:
	APlayerCharacter();

	// Movement Events.
	UFUNCTION(BlueprintCallable) void JumpEvent          ();
	UFUNCTION(BlueprintCallable) void StartRunningEvent  ();
	UFUNCTION(BlueprintCallable) void StopRunningEvent   ();
	UFUNCTION(BlueprintCallable) void StartDashingEvent  ();
	UFUNCTION(BlueprintCallable) void StopDashingEvent   ();
	UFUNCTION(BlueprintCallable) void StartSuperjumpEvent();
	UFUNCTION(BlueprintCallable) void StopSuperjumpEvent ();

	// Bow Events.
	UFUNCTION(BlueprintCallable) void StartAimingEvent  ();
	UFUNCTION(BlueprintCallable) void StopAimingEvent   ();
	UFUNCTION(BlueprintCallable) void ShootEvent        ();
	UFUNCTION(BlueprintCallable) void PickupArrowsEvent (const int& Amount);

	// Misc Events.
	UFUNCTION(BlueprintCallable) void TakeDamageEvent    (const float& Amount);
	UFUNCTION(BlueprintCallable) void HealEvent          (const float& Amount);
	UFUNCTION(BlueprintCallable) void InvincibilityEvent (const float& Duration);
	UFUNCTION(BlueprintCallable) void UseStaminaEvent    (const float& Amount);
	UFUNCTION(BlueprintCallable) void AddStaminaEvent    (const float& Amount);
	UFUNCTION(BlueprintCallable) void AddScoreEvent      (const float& Amount);
	UFUNCTION(BlueprintCallable) void DeathEvent         ();
	UFUNCTION(BlueprintCallable) void StartReanimateEvent();
	UFUNCTION(BlueprintCallable) void StopReanimateEvent ();

	// Utilities.
	UFUNCTION(BlueprintCallable) void StartFovTransition(const float& TargetFov, const float& Duration);
	UFUNCTION(BlueprintCallable) void LockRotationToCam (const bool& Locked) const;

	// Player state getters.
	UFUNCTION(BlueprintCallable)             bool CanRun           () const;
	UFUNCTION(BlueprintCallable)             bool CanDash          () const;
	UFUNCTION(BlueprintCallable)             bool CanSuperjump     () const;
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsRunning        () const { return Running; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsDashing        () const { return DashTimer.HasStarted(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsSuperjumping   () const { return SuperjumpTimer.HasStarted(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsAiming         () const { return AimingTimer.HasStarted() && !AimingCancelled; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsShooting       () const { return ShootFramesCounter > 0; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsInvincible     () const { return InvincibilityTimer.HasStarted(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsReanimating    () const { return ReanimationTimer.HasStarted(); }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsDead           () const { return Health <= 0; }
	UFUNCTION(BlueprintCallable)             bool IsOtherPlayerDead();

	// Misc getters.
	UFUNCTION(BlueprintCallable) FORCEINLINE int               GetPlayerIndex     () const { return PlayerIndex; }
	UFUNCTION(BlueprintCallable) FORCEINLINE int               GetScore           () const { return Score; }
	UFUNCTION(BlueprintCallable) FORCEINLINE int               GetArrowCount      () const { return ArrowCount; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float             GetAimingCompletion() const { return FMath::Clamp(AimingTimer.GetCurrentTime() / MaxAimingDuration, 0.f, 1.f); }
	UFUNCTION(BlueprintCallable)             APlayerCharacter* GetOtherPlayer     ();
	UFUNCTION(BlueprintCallable) FORCEINLINE UCameraComponent* GetCamera          () const { return CameraComponent; }
	UFUNCTION(BlueprintCallable) FORCEINLINE AGameHUD*         GetGameHud         () const { return GameHud; }

};
