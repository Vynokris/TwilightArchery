#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TwilightArchery/UI/GameHUD.h"
#include "BossEnterBox.generated.h"

UCLASS()
class TWILIGHTARCHERY_API ABossEnterBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossEnterBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION() void WidgetAdd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere) UBoxComponent* Trigger;
	UPROPERTY(VisibleAnywhere) AGameHUD* Hud;
};
