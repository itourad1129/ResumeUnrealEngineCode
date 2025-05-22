// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Character/Player/PlayerData.h"

void UPlayerData::Init(const uint32 InID,const FString& InName)
{
	this->ID = InID;
	Super::Init(InName,ECharacterType::Player);
}
