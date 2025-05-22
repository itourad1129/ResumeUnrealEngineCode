

#pragma once

#include "CoreMinimal.h"
#include "Asset/Item/Equip/Human/HumanEquipItemDataTablePAL.h"
#include "Http/RequestMasterDataVersion.h"
#include "Item/Equip/Human/HumanEquipItemData.h"
#include "UObject/Object.h"
class UOriginalGameInstance;
#include "MasterDataManager.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMasterDataDownloadCompleted);
UCLASS( config = Game, defaultconfig )
class PJDRC_API UMasterDataManager : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FMasterDataVersion> MasterDataVersion;
	bool MasterDataVersionDownloaded = false;
	
	UPROPERTY(BlueprintAssignable, Category = "Chunk")
	FOnMasterDataDownloadCompleted OnMasterDataDownloadCompleted;
	void Init();
	// マスターデータダウンロード
	void DownLoadMasterData(int64 ChunkID,UOriginalGameInstance* GameIns);
	void SetMasterData(bool DownloadComplete);

	// マスターデータアセットのファイルパス
	//人間用装備品マスターデータ
	UPROPERTY( EditAnywhere, Config, meta = (AllowedClasses = "/Script/PJDRC.HumanEquipItemDataTableDataAsset") )
	FSoftObjectPath HumanEquipItemDataTableDataAssetPath;
	UPROPERTY( EditAnywhere, Config, meta = (AllowedClasses = "/Script/PJDRC.HumanEquipItemDataTablePAL") )
	FSoftObjectPath HumanEquipItemDataTablePALPath;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHumanEquipItemDataTableDataAsset> HumanEquipItemDataTableDataAsset;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHumanEquipItemDataTablePAL> HumanEquipItemDataTablePAL;
private:
	int32 DownloadTable;
	int32 DownloadedTable;
};
