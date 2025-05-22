// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UserSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API UUserSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString TransferCode;
	
	UPROPERTY()
	int64 UserID;

	UPROPERTY()
	FString Token;
};
