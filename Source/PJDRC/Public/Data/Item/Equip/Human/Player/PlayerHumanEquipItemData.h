// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/Equip/EquipItemData.h"
#include "Data/Item/Equip/Human/HumanEquipItemData.h"
#include "PlayerHumanEquipItemData.generated.h"
/**
 * 
 */
UCLASS()
class PJDRC_API UPlayerHumanEquipItemData : public UHumanEquipItemData
{
	GENERATED_BODY()
protected:
	using UHumanEquipItemData::Init;
	virtual void Init(EHumanSlotType InType,const FString& InName) override;
private:
};
