#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseMenu.generated.h"

UCLASS()
class TWILIGHTARCHERY_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget)) UButton* ResumeButton;
	UPROPERTY(meta = (BindWidget)) UButton* RestartButton;
	UPROPERTY(meta = (BindWidget)) UButton* QuitButton;
	
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable) void Pause();
	UFUNCTION(BlueprintCallable) void Resume();
	UFUNCTION(BlueprintCallable) void Restart();
	UFUNCTION(BlueprintCallable) void Quit();

	
};
