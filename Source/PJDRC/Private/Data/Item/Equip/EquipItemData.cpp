// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Item/Equip/EquipItemData.h"

void UEquipItemData::Init(const EEquipSlotType InType,const FString& InName)
{
	this->Type = InType;
	this->Name = InName;
}

//ここにChunkからマスターをDLする処理を記載。マスターは一括DLのがいい。