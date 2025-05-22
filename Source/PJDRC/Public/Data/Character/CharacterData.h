// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CharacterData.generated.h"

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	None = 0    UMETA(DisplayName = "None"),
	Player = 0    UMETA(DisplayName = "Player"),
	Enemy = 0    UMETA(DisplayName = "Enemy"),
	NPC = 0    UMETA(DisplayName = "NPC"),
};
/**
 * 
 */
UCLASS()
class PJDRC_API UCharacterData : public UObject
{
	GENERATED_BODY()
public:
	virtual void Init(const FString& InName, ECharacterType InType);
protected:
	UPROPERTY(Transient, EditAnywhere)
	ECharacterType Type;
	UPROPERTY(Transient, EditAnywhere)
	FString Name;
};
