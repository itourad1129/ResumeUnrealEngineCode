// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "OriginalAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API UOriginalAssetManager : public UAssetManager
{
	GENERATED_BODY()
	/**
	*	@brief	Get MyAssetManager.
	*/
public:
	static UOriginalAssetManager& GetAssetManager();
private:
	//static const FPrimaryAssetType CharacterDataItemType;
};
