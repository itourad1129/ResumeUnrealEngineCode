// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/OriginalAssetManager.h"
#include "Utility/OriginalPAL.h"
#include "HumanEquipPAL.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API UHumanEquipPAL : public UOriginalPAL
{
	GENERATED_BODY()
public:
	//スケルタルメッシ
	UPROPERTY(Transient, EditDefaultsOnly)
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;
};
