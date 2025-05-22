// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/PrimaryAssetLabel.h"
#include "OriginalPAL.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API UOriginalPAL : public UPrimaryAssetLabel
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPrimaryAssetId PrimaryAssetId = GetPrimaryAssetId();
};
