// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OriginalPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API UOriginalPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	/**
	*	@brief	プライマリアセットID名を取得
	*/
	UFUNCTION(BlueprintCallable)
	FString GetIdentifierString() const;


	/**
	*	@brief	プライマリアセットIDを取得
	*/
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;


	// アイテムタイプ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPrimaryAssetType ItemType;
};
