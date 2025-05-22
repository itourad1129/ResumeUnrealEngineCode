// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "TitleScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API UTitleScreenWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StartButtonText;
};
