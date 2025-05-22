// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Widget/Title/TitleScreenWidget.h"
#include "TitleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API ATitleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void Init_TitleScreen();
	void StartMatching();
	
	UFUNCTION()
	void OnStartButtonClicked();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> TitleScreenWidgetClass;
private:
	UPROPERTY()
	UTitleScreenWidget* TitleScreenWidget;
	bool IsNewGame;
};
