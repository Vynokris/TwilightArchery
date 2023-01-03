#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "TwilightArchery/Utils/Cooldown.h"
#include "Boss.generated.h"
class APlayerCharacter;
class AGameHUD;
class UBehaviorTree;
class UBoxComponent;

UCLASS()
class TWILIGHTARCHERY_API ABoss : public APawn
{
	GENERATED_BODY()
	
protected:
	// Base Meshes.
	UPROPERTY(EditAnywhere, Category = "Mesh") USkeletalMeshComponent* BossMesh;
	UPROPERTY(EditAnywhere, Category = "Mesh") USkeletalMeshComponent* BossHeadMesh;
	
	//	Weak Points Colliders.
	UPROPERTY(EditAnywhere, Category = "Weak Points") UBoxComponent* EyeWeakPoint;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UBoxComponent* ArmWeakPoint1;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UBoxComponent* ArmWeakPoint2;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UBoxComponent* ArmWeakPoint3;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UBoxComponent* ArmWeakPoint4;

	// Weak Points Glow.
	UPROPERTY(EditAnywhere, Category = "Weak Points") UStaticMeshComponent* EyeWeakPointGlow;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UStaticMeshComponent* ArmWeakPointGlow1;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UStaticMeshComponent* ArmWeakPointGlow2;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UStaticMeshComponent* ArmWeakPointGlow3;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UStaticMeshComponent* ArmWeakPointGlow4;

	// Weak Points Materials.
	UPROPERTY(EditAnywhere, Category = "Weak Points") UMaterialInstance* WeakPointBlinkEnragedMaterial = nullptr;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UMaterialInstance* WeakPointBlinkBasicMaterial   = nullptr;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UMaterialInstance* WeakPointEnragedMaterial      = nullptr;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UMaterialInstance* WeakPointStunnedMaterial      = nullptr;
	UPROPERTY(EditAnywhere, Category = "Weak Points") UMaterialInstance* WeakPointBasicMaterial        = nullptr;

	// Behavior Tree.
	UPROPERTY(EditAnywhere, Category = "AI") UBehaviorTree* BehaviorTree;
	
	// HUD.
	AGameHUD* Hud;

	// Blinking.
	UPROPERTY(EditAnywhere, Category = "Health") float DamageBlinkDuration = 1.5;
	FCooldown DamageBlinkCooldown = { 0 };
	
	// Health Variables.
	UPROPERTY(EditAnywhere, Category = "Health") float MaxHealth = 160;
	float Health;

	// Focused Player.
	APlayerCharacter* FocusedPlayer         = nullptr;
	APlayerCharacter* Players[2]            = { nullptr, nullptr };
	float             DamageDoneByPlayer[2] = { 0, 0 };

	// State variables.
	bool Enabled = false;
	bool Enraged = false;
	bool Stunned = false;

	// Body rotation.
	int   RotationDir   = 1;
	float RotationSpeed = 4;

	// Sound
	UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* DeathSound = nullptr;
	UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* WeakPointSound = nullptr;
	UPROPERTY(EditAnywhere, Category = "Sound") USoundBase* HitBodySound[3];


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	bool AreWeakPointsValid() const;
	void SetWeakPointsMaterial(UMaterialInstance* Material) const;

public:
	ABoss();

	UFUNCTION(BlueprintCallable) void SetEnabled     (const bool& Value);
	UFUNCTION(BlueprintCallable) void SetEnraged     (const bool& Value);
	UFUNCTION(BlueprintCallable) void SetStunned     (const bool& Value);
	UFUNCTION(BlueprintCallable) void TakeDamageEvent(const float& Damage, const int& AttackingPlayerId, const bool& HitWeakPoint = false);
	UFUNCTION(BlueprintCallable) void DeathEvent     ();
	
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsEnabled   () const { return Enabled;   }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsEnraged   () const { return Enraged;   }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool IsStunned   () const { return Stunned;   }
	UFUNCTION(BlueprintCallable) FORCEINLINE int  GetHealth   () const { return Health;    }
	UFUNCTION(BlueprintCallable) FORCEINLINE int  GetMaxHealth() const { return MaxHealth; }
	
	UFUNCTION(BlueprintCallable)             void              GetPlayers      ();
	UFUNCTION(BlueprintCallable) FORCEINLINE APlayerCharacter* GetFocusedPlayer() const { return FocusedPlayer; }
	UFUNCTION(BlueprintCallable) FORCEINLINE UBehaviorTree*    GetBehaviorTree () const { return BehaviorTree; }
};
