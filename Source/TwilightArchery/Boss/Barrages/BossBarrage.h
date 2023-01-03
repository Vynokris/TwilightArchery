

#pragma once

#include "CoreMinimal.h"
#include "../../Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "TwilightArchery/Boss/Boss.h"
#include "BossBarrage.generated.h"

UCLASS()
class TWILIGHTARCHERY_API ABossBarrage : public AActor
{
	GENERATED_BODY()

	ABossBarrage();
	ABossBarrage(FVector _Target);
	
	// PROPERTIES
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara") UNiagaraComponent* Niagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Data") float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Data") FVector Target;
	
	// FUNCTIONS
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
