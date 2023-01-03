#include "TwilightArcheryGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "GameMapsSettings.h"	
#include "Player/PlayerCharacter.h"
#include "Player/PlayersSpawnPoint.h"
#include "Player/TwilightArcheryPlayerController.h"
#include "Utils/DebugUtils.h"

void ATwilightArcheryGameModeBase::BeginPlay()
{
	Super::BeginPlay();

    if(!UGameplayStatics::GetPlayerController(GetWorld(), 1))
		UGameplayStatics::CreatePlayer(GetWorld());
	
	APlayerController* PlayerMode = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	
	if(PlayerMode) {PlayerMode->SetInputMode(FInputModeGameOnly());}

	const TSubclassOf<APlayersSpawnPoint> ClassToFind = APlayersSpawnPoint::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind,PlayerStartArray);
	for (int i = 0; i < PlayerStartArray.Num(); i++)
		SpawnPlayer(Cast<ATwilightArcheryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), i)), i);
}
void ATwilightArcheryGameModeBase::SpawnPlayer(ATwilightArcheryPlayerController* Controller, const int& PlayerIndex) const
{
	if(!IsValid(Controller)) return;
	
	APlayerCharacter* OutValue = nullptr;
	UGameplayStatics::SpawnObject(DefaultPawnClass, OutValue);
	
	Controller->Possess(OutValue);
}

