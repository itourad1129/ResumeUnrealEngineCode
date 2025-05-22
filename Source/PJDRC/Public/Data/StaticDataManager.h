// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
class UOriginalGameInstance;
#include "StaticDataManager.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API UStaticDataManager : public UObject
{
	GENERATED_BODY()
public:
	void Init();
};