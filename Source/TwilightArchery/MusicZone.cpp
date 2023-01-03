


#include "MusicZone.h"
#include "Player/PlayerCharacter.h"
#include "Components/AudioComponent.h"

// Sets default values
AMusicZone::AMusicZone()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Play zone"));
	Box->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AMusicZone::PlayMusic(int ID)
{
	if (ID >= MusicList.Num())
		return;
	
	MusicList[CurrentMusic]->Deactivate();

	MusicList[ID]->Activate(true);
}

void AMusicZone::PlayRandom()
{
	if (!(MusicList.Num() > 0))
		return;
	
	MusicList[CurrentMusic]->Deactivate();

	const int ToPlay = FMath::RandRange(0, MusicList.Num() - 1);

	CurrentMusic = ToPlay;
	MusicList[ToPlay]->Activate();
}

void AMusicZone::BeginPlay()
{
	const TSubclassOf<UAudioComponent> ClassToFind = UAudioComponent::StaticClass();
	TArray<UActorComponent*> OutArray  = GetComponentsByClass(ClassToFind);

	if (!(OutArray.Num() > 0))
		return;

	for (int i = 0; i < OutArray.Num(); i++)
	{
		OutArray[i]->Deactivate();
		MusicList.Push(Cast<UAudioComponent>(OutArray[i]));
	}
	
	CurrentMusic = 0;
	
	if (PlayRandomOnStart)
		PlayRandom();
}

void AMusicZone::PlayNextMusic()
{
	if (!(MusicList.Num() > 0))
		return;
	
	MusicList[CurrentMusic]->Deactivate();

	const int NewMusic = (CurrentMusic + 1) % MusicList.Num();
	CurrentMusic = NewMusic;
	MusicList[NewMusic]->Activate(true);
}

void AMusicZone::Pause()
{
	MusicList[CurrentMusic]->SetPaused(true);
}

void AMusicZone::Resume()
{
	MusicList[CurrentMusic]->SetPaused(false);
}

void AMusicZone::Tick(float DeltaSeconds)
{
	if (!MusicList[CurrentMusic]->IsPlaying())
		PlayNextMusic();
}

void AMusicZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!Cast<APlayerCharacter>(OtherActor))
		return;

	PlayRandom();
}

void AMusicZone::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (!Cast<APlayerCharacter>(OtherActor))
		return;

	MusicList[CurrentMusic]->Deactivate();
}

