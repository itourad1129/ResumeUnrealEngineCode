// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Character/CharacterData.h"
#include "UObject/Object.h"
#include "PlayerData.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API UPlayerData : public UCharacterData
{
	GENERATED_BODY()
protected:
	using UCharacterData::Init;
	void Init(uint32 InID,const FString& InName);
private:
	uint32 ID;
};
