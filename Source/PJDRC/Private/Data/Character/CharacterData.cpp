// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Character/CharacterData.h"

void UCharacterData::Init(const FString& InName, const ECharacterType InType)
{
	this->Type = InType;
	this->Name = InName;
}
