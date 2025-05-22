// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryDataAsset/OriginalPrimaryDataAsset.h"
/******************************************************************************
*	@brief	プライマリアセットID名を取得
*******************************************************************************/
FString UOriginalPrimaryDataAsset::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}






/******************************************************************************
*	@brief	プライマリアセットIDを取得
*******************************************************************************/
FPrimaryAssetId UOriginalPrimaryDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}