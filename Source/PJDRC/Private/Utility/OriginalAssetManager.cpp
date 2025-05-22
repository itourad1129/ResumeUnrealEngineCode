// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/OriginalAssetManager.h"

UOriginalAssetManager& UOriginalAssetManager::GetAssetManager()
{
	UOriginalAssetManager* AssetManager = Cast< UOriginalAssetManager>(GEngine->AssetManager);
	if (AssetManager)
	{
		return *AssetManager;
	}
	else
	{
		return (*NewObject<UOriginalAssetManager>());
	}
}
