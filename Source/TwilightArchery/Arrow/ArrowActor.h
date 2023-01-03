#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowActor.generated.h"
class UProjectileMovementComponent;
class APlayerCharacter;

UCLASS()
class TWILIGHTARCHERY_API AArrowActor : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Arrow Components") UShapeComponent*              BoxComponent       = nullptr;
	UPROPERTY(EditAnywhere, Category = "Arrow Components") UStaticMeshComponent*         MeshComponent      = nullptr;
    UPROPERTY(EditAnywhere, Category = "Arrow Components") UParticleSystemComponent*     ParticleComponent  = nullptr;
	UPROPERTY(EditAnywhere, Category = "Arrow Components") UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(EditAnywhere, Category = "Arrow Parameters") float DespawnDuration = 5;
	UPROPERTY(EditAnywhere, Category = "Arrow Parameters") float Damage = 1;

	APlayerCharacter* ParentPlayer = nullptr;

public:	
	AArrowActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:	
	void SetDamage      (const float& NewDamage  ) { Damage = NewDamage; }
	void SetParentPlayer(APlayerCharacter* Player) { ParentPlayer = Player; }
	
	UFUNCTION(BlueprintCallable) void ActivateTrail() const;
	
	UFUNCTION(BlueprintCallable) FORCEINLINE UShapeComponent*              GetBox()                const { return BoxComponent; }
	UFUNCTION(BlueprintCallable) FORCEINLINE UStaticMeshComponent*         GetMesh()               const { return MeshComponent; }
	UFUNCTION(BlueprintCallable) FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	UFUNCTION(BlueprintCallable) FORCEINLINE float                         GetDamage()             const { return Damage; }
	UFUNCTION(BlueprintCallable) FORCEINLINE APlayerCharacter*             GetParentPlayer()       const { return ParentPlayer; }
};
