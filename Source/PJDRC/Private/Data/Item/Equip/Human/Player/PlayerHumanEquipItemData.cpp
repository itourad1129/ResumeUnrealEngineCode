// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/Item/Equip/Human/Player/PlayerHumanEquipItemData.h"

void UPlayerHumanEquipItemData::Init(const EHumanSlotType InType,const FString& InName)
{
	HumanSlotType = InType;
	HumanOwnerType = EHumanOwnerType::Player;
	Super::Init(EEquipSlotType::Human,InName);
}
