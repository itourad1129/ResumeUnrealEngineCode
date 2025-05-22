// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OriginalGameInstance.h"
#include "UObject/Object.h"
#include "EquipItemAssetData.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API UEquipItemAssetData : public UObject
{
	GENERATED_BODY()
public:
	void SetEquipAssetData(UOriginalGameInstance* GameIns, USkeletalMeshComponent* SkMeshCmp, TSoftObjectPtr<USkeletalMesh> SkMesh, int32 ChunkID);
};
