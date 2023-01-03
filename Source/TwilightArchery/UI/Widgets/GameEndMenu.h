#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameEndMenu.generated.h"

UCLASS()
class TWILIGHTARCHERY_API UGameEndMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget)) UButton* RestartButton;
	UPROPERTY(meta = (BindWidget)) UButton* QuitButton;
	
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION(BlueprintCallable) void Restart();
	UFUNCTION(BlueprintCallable) void Quit();
	UFUNCTION(BlueprintCallable) void ShowMenu();
};
