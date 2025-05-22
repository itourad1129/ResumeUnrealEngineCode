// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chunk.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPatchCompleteDelegate, bool, Succeeded);
/**
 * 
 */
UCLASS(config=Game, transient)
class PJDRC_API UChunk : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Config)
	bool Visible;
	UPROPERTY(BlueprintAssignable, Category="Patching");
	FPatchCompleteDelegate OnPatchComplete;
	bool IsPatchingInProgress;
	int32 DownloadingChunkId = -1;
	int32 LastChunkStatus = -1;
	void Init(FString Deployment);
	void ShutDown();
	void PatchGameTask(int32 chunkId,bool DlManifestUptoDate,TFunction<void(void)> CallBack);
	bool CheckMountComplete(int32 chunkId);
	void ChunkDownloadAssetAsync(int32 ChunkId,TFunction<void(void)> CallBack);
	void ProcessChunkDownloadQueue();
	void EnqueueChunkDownloadTask(int32 ChunkId, TFunction<void()> CallBack);
	void CheckChunkDownloadNextTask();
	void PrintStringStandalone(const FString& Message);
private:
	struct FChunkDownloadTask
	{
		int32 ChunkId;
		TFunction<void(void)> Callback;
		FChunkDownloadTask()
		: ChunkId(0) {}
		FChunkDownloadTask(int32 InChunkId, TFunction<void(void)> InCallback)
		: ChunkId(InChunkId), Callback(MoveTemp(InCallback)) {}
	};
	TQueue<FChunkDownloadTask> DlTaskList;
	bool IsChunkDownloading = false;
	FString DeploymentName = "";
	FString ContentBuildId = "";
};
