

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MusicZone.generated.h"

UCLASS()
class TWILIGHTARCHERY_API AMusicZone : public AActor
{
	GENERATED_BODY()
	

protected:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components") UBoxComponent* Box = nullptr;   

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	TArray<UAudioComponent*> MusicList;
	
	int CurrentMusic;
	
public:	
	AMusicZone();

	void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "Music")
	bool PlayRandomOnStart;

	UFUNCTION(BlueprintCallable)
	void PlayNextMusic();
	
	UFUNCTION(BlueprintCallable)
	void Pause();

	UFUNCTION(BlueprintCallable)
	void Resume();

	UFUNCTION(BlueprintCallable)
	void PlayMusic(int ID);

	UFUNCTION(BlueprintCallable)
	void PlayRandom();
};
