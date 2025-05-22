#include "Data/MasterDataManager.h"
#include "OriginalGameInstance.h"
#include "Http/RequestMasterDataVersion.h"
#include "Data/Item/Equip/Human/HumanEquipItemData.h"
#include "Kismet/KismetSystemLibrary.h"

void UMasterDataManager::Init()
{
	if(!MasterDataVersionDownloaded)
	{
		TFunction<void(bool)> DownloadCompleteCallback = [this](bool bSuccess)
		{
			SetMasterData(bSuccess);
		};
		TFunction<void(TArray<FMasterDataVersion>)> ResponseCallback = [this,DownloadCompleteCallback](TArray<FMasterDataVersion> response)
		{
			MasterDataVersion = response;
			if (response.Num() > 0)
			{
				DownloadTable = response.Num();
				MasterDataVersionDownloaded = true;
				DownloadCompleteCallback(true);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("HTTP Request Failed or JSON Parsing Error"));
				DownloadCompleteCallback(false);
			}
		};
		URequestMasterDataVersion* Request = NewObject<URequestMasterDataVersion>(this);
		Request->RequestMasterDataVersion(ResponseCallback);
	}
}

void UMasterDataManager::DownLoadMasterData(int64 ChunkID, UOriginalGameInstance* GameIns)
{
	TFunction<void(void)> CallBack = [this](void)
	{
		DownloadedTable++;
		if (DownloadTable == DownloadedTable)
		{
			OnMasterDataDownloadCompleted.Broadcast();
			UE_LOG(LogTemp, Log, TEXT("MasterData Download Complete!"));
		}
		FHumanEquipDataTable humanEquipData;
		UHumanEquipItemDataTableDataAsset::GetHumanEquipData(GetWorld(),1,humanEquipData);
		UKismetSystemLibrary::PrintString(this,humanEquipData.Name, true, true, FColor::Cyan, 30.f, TEXT("None"));
		UE_LOG(LogTemp, Log, TEXT("装備名: %s"), *humanEquipData.Name);
	};
	GameIns->Chunk->EnqueueChunkDownloadTask(ChunkID,CallBack);
}

void UMasterDataManager::SetMasterData(bool downloadComplete)
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if(GameInstance != nullptr && downloadComplete){
		UOriginalGameInstance* OriginalGameInstance = Cast<UOriginalGameInstance>(GameInstance);
		if (OriginalGameInstance != nullptr)
		{
			DownloadTable = 0;
			DownloadedTable = 0;
			//ダウンロードするマスターデータ一覧
			if (MasterDataVersion.Num() > 0)
			{
				DownloadTable = MasterDataVersion.Num();
				for (const FMasterDataVersion& Data : MasterDataVersion)
				{
					DownLoadMasterData(Data.ChunkID,OriginalGameInstance);
					UE_LOG(LogTemp, Log, TEXT("MasterDataID: %d, ChunkID: %d"), Data.MasterDataID, Data.ChunkID);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("MasterDataVersion is empty or not initialized."));
			}
		}
	}
	// else
	// {
	// 	//マスターデータダウンロードエラー初期記載予定
	// }
}
