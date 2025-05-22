// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk/Chunk.h"
#include "ChunkDownloader.h"
#include "Http/RequestChunkVersion.h"
#include "Kismet/KismetSystemLibrary.h"

void UChunk::Init(FString Deployment)
{
	FString PlatformName;
#if PLATFORM_WINDOWS
	PlatformName = "Windows";
#elif PLATFORM_MAC
	PlatformName = "Mac";
#elif PLATFORM_IOS
	PlatformName = "iOS";
#elif PLATFORM_ANDROID
	PlatformName = "Android";
#else
	PlatformName = "Unknown";
#endif
	//以下ChunkDLの初期化処理は通信基盤ができ次第別の関数にまとめる予定
	//DeploymentName = "PatchingDemoLive";
	//ContentBuildId = "//storage.googleapis.com/dl-asset-storage/PatchingDemoLive/";
	// initialize the chunk downloader
	TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetOrCreate();
	Downloader->Initialize(PlatformName, 8);
	// load the cached build ID
	bool buildCahe = Downloader->LoadCachedBuild(Deployment);
	if(buildCahe)
	{
		PrintStringStandalone(TEXT("buildCache:true"));
	}
	else
	{
		PrintStringStandalone(TEXT("buildCache:false"));
	}
}

void UChunk::ShutDown()
{
	FChunkDownloader::Shutdown();
	DlTaskList.Empty();
}

void UChunk::PatchGameTask(int32 ChunkId, bool DlManifestUptoDate, TFunction<void()> CallBack)
{
	if (DlManifestUptoDate)
	{
		TFunction<void (bool DlSuccess)> DownloadCompleteCallback = [this,ChunkId,CallBack](bool DlSuccess)
		{
			if (DlSuccess)
			{
				UE_LOG(LogTemp, Display, TEXT("Download complete"));
				//Mount the chunks
				TFunction<void(bool bSuccess)> MountCompleteCallback = [this,ChunkId,CallBack](bool bSuccess)
				{
					if(bSuccess)
					{
						FString PriString = FString::Printf(TEXT("MountComplete! = %d"),  ChunkId);
						PrintStringStandalone(PriString);
						if(CheckMountComplete(ChunkId))
						{
							CallBack();
						}
						OnPatchComplete.Broadcast(bSuccess);
					}
					else
					{
						CheckChunkDownloadNextTask();
					}
				};
				TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetChecked();
				Downloader->MountChunk(ChunkId, MountCompleteCallback);
				OnPatchComplete.Broadcast(true);
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Load process failed"));
				// call the delegate
				OnPatchComplete.Broadcast(false);
				CheckChunkDownloadNextTask();
			}
		};
		TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetChecked();
		Downloader->DownloadChunk(ChunkId, DownloadCompleteCallback, 1);
		IsPatchingInProgress = true;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Manifest Update Failed.Can't patch the game"));
		PrintStringStandalone(TEXT("Manifest Update Failed.Can't patch the game"));
		CheckChunkDownloadNextTask();
	}
}

bool UChunk::CheckMountComplete(int32 chunkId)
{
	TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetChecked();
	if(FChunkDownloader::EChunkStatus::Mounted == Downloader->GetChunkStatus(chunkId) || FChunkDownloader::EChunkStatus::Cached == Downloader->GetChunkStatus(chunkId))
	{
		// FString PriString = FString::Printf(TEXT("MountComplete! = %d"), chunkId);
		// UKismetSystemLibrary::PrintString(this,PriString, true, true, FColor::Cyan, 30.f, TEXT("None"));
		DownloadingChunkId = -1;
		IsPatchingInProgress = false;
		return true;
	}
	else
	{
		int32 ChunkStatus = static_cast<int32>(Downloader->GetChunkStatus(chunkId));
		if(LastChunkStatus != ChunkStatus)
		{
			FString PriString = FString::Printf(TEXT("ChunkState = %d"), ChunkStatus);
			PrintStringStandalone(PriString);
		}
		LastChunkStatus = ChunkStatus;
	}
	return false;
}

void UChunk::ChunkDownloadAssetAsync(int32 ChunkId, TFunction<void()> CallBack)
{
	TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetOrCreate();
	TFunction<void(bool bSuccess)> UpdateCompleteCallback = [this,ChunkId,CallBack](bool bSuccess)
	{
		PatchGameTask(ChunkId,bSuccess,CallBack);
	};
	if(DeploymentName == "" || ContentBuildId == "")
	{
		TFunction<void(FChunkVersionData response)> ResponseCallback = [this,UpdateCompleteCallback](FChunkVersionData response)
		{
			this->DeploymentName = response.DeploymentName;
			this->ContentBuildId = response.ContentBuildID;
			TSharedRef<FChunkDownloader> Downloader = FChunkDownloader::GetOrCreate();
			this->Init(response.DeploymentName);
			Downloader->UpdateBuild(response.DeploymentName, response.ContentBuildID, UpdateCompleteCallback);
		};
		URequestChunkVersion* Request = NewObject<URequestChunkVersion>(this);
		Request->RequestChunkVersion(ResponseCallback);
	}
	else
	{
		Downloader->UpdateBuild(DeploymentName, ContentBuildId, UpdateCompleteCallback);
	}
}

void UChunk::ProcessChunkDownloadQueue()
{
	if (IsChunkDownloading || DlTaskList.IsEmpty())
	{
		return; // 現在ダウンロード中またはキューが空の場合、何もしない
	}
	FChunkDownloadTask NextTask;
	if (DlTaskList.Dequeue(NextTask))
	{
		IsChunkDownloading = true;

		// ダウンロード処理を開始
		ChunkDownloadAssetAsync(NextTask.ChunkId, [this,NextTask]()
		{
			// ダウンロードが完了したら、元々のコールバックを呼び出す
			NextTask.Callback();
			// ダウンロードが完了したので、次のタスクを処理
			IsChunkDownloading = false;
			ProcessChunkDownloadQueue();
		});
	}
}

void UChunk::EnqueueChunkDownloadTask(int32 ChunkId, TFunction<void()> CallBack)
{
	if(Visible)
	{
		DlTaskList.Enqueue(FChunkDownloadTask(ChunkId, MoveTemp(CallBack)));
		ProcessChunkDownloadQueue();
	}
	else
	{
		CallBack();
	}
}

void UChunk::CheckChunkDownloadNextTask()
{
	FChunkDownloadTask NextTask;
	// キューの次のタスクを削除し、次のタスクがあるかどうかを確認
	if (DlTaskList.Dequeue(NextTask))
	{
		// 次のタスクがあれば、次のタスクを処理
		IsChunkDownloading = true;
		//今はChunkDownloadAssetAsyncを呼び出すが、通信基盤ができたら、DeploymentName、ContentBuildId初期化用の関数に変更し、その中で
		//ChunkDownloadAssetAsyncをコールバックにして呼び出す
		ChunkDownloadAssetAsync(NextTask.ChunkId, NextTask.Callback);
	}
	else
	{
		// キューが空であれば、ダウンロード中フラグをリセット
		IsChunkDownloading = false;
	}
}

void UChunk::PrintStringStandalone(const FString& Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Message);
	}
}
