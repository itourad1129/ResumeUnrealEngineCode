// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chunk/Chunk.h"
#include "Data/MasterDataManager.h"
#include "Engine/GameInstance.h"
#include "Data/StaticDataManager.h"
#include "OriginalGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class PJDRC_API UOriginalGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	TObjectPtr<UMasterDataManager> MasterDataManager;
	UPROPERTY()
	TObjectPtr<UChunk> Chunk;
	UPROPERTY()
	bool bIsChunkInitialized = false;
	bool PlayerSpawned = false;
	virtual void Init() override;
	virtual void Shutdown() override;

	//Level移動
	UFUNCTION(BlueprintCallable)
	//Level移動後に呼ばれる
	void LoadedEvent(UWorld* LoadedWorld);
	void ConnectServer(FString ServerAddress);
};
