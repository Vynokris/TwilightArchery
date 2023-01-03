


#include "BossBarrage.h"

// Sets default values
ABossBarrage::ABossBarrage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
}

ABossBarrage::ABossBarrage(FVector _Target)
{
	ABossBarrage();
	Target=_Target;
}


// Called when the game starts or when spawned
void ABossBarrage::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ABossBarrage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

