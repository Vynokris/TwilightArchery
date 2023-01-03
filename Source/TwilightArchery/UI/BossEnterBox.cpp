#include "BossEnterBox.h"

#include "Kismet/GameplayStatics.h"
#include "TwilightArchery/Boss/Boss.h"
#include "TwilightArchery/Player/PlayerCharacter.h"

// Sets default values
ABossEnterBox::ABossEnterBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));

}

// Called when the game starts or when spawned
void ABossEnterBox::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this,&ABossEnterBox::WidgetAdd);
	Hud = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// Called every frame
void ABossEnterBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossEnterBox::WidgetAdd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if(OtherActor->Tags.Contains("Player"))
	{
		Hud->BossHealthBar->SetVisibility(ESlateVisibility::Visible);

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Boss", FoundActors);
		if (FoundActors.Num() > 0)
			Cast<ABoss>(FoundActors[0])->SetEnabled(true);
	}
}


