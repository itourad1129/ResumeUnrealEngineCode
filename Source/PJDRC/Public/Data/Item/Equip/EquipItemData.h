// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EquipItemData.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EEquipSlotType : uint8
{
	None UMETA(DisplayName = "None"),
	Human UMETA(DisplayName = "Human"),
};
UCLASS()
class PJDRC_API UEquipItemData : public UObject
{
	GENERATED_BODY()
protected:
	virtual void Init(EEquipSlotType InType,const FString& InName);
	UPROPERTY(Transient, EditAnywhere)
	EEquipSlotType Type;
	UPROPERTY(Transient, EditAnywhere)
	FString Name;
};
