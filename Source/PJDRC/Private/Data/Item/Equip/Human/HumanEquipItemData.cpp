// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/Item/Equip/Human/HumanEquipItemData.h"

#include "OriginalGameInstance.h"

#if WITH_EDITOR
#include "Misc/MessageDialog.h"
#include "UObject/Package.h"
#include "EditorReimportHandler.h"
#include "FileHelpers.h"
#endif

void UHumanEquipItemDataTableDataAsset::Build()
{
#if WITH_EDITORONLY_DATA
 
	// データテーブルの設定チェック
	if( DataTable == nullptr )
	{
		FMessageDialog::Open(EAppMsgType::Ok,FText::FromString(TEXT("DataTable is Null !!")));
		return;
	}
 
	// データテーブルの型チェック
	if( !DataTable->GetRowStruct()->IsChildOf( FHumanEquipDataTableRowBase::StaticStruct() ) )
	{
		FMessageDialog::Open(EAppMsgType::Ok,FText::FromString(TEXT("DataTable type does not match !!")));
		return;
	}
 
	TArray<UPackage*> PackagesToSave;
 
	// データテーブルをリインポート
	if( FReimportManager::Instance()->Reimport( DataTable, false, true ) )
	{
		// リインポートに成功したデータテーブルを保存対象に追加
		PackagesToSave.Add( DataTable->GetOutermost() );
	}
 
	HumanEquipDataMap.Empty();
 
	// データテーブルの行の要素を配列で取得
	TArray RowNames = DataTable->GetRowNames();
	// 行の数だけループ
	for( auto RowName : RowNames )
	{
		// １行分の構造体を取得
		FHumanEquipDataTableRowBase* TableRow = DataTable->FindRow< FHumanEquipDataTableRowBase >( RowName, FString() );
 
		// 実際にゲーム上で使いやすいようにデータを加工する
		FHumanEquipDataTable HumanEquipData;
		HumanEquipData.ID = TableRow->ID;
		HumanEquipData.SlotType = TableRow->SlotType;
		HumanEquipData.Name= TableRow->Name;
		HumanEquipData.ChunkID = TableRow->ChunkID;
 
		// Mapに追加する
		HumanEquipDataMap.Add( TableRow->ID, HumanEquipData );
	}
 
	// データアセットに編集フラグを追加
	//MarkPackageDirty();
 
	// データアセットを保存対象に追加
	PackagesToSave.Add( GetOutermost() );
 
	// 関連アセットを全て保存（SourceControl使用時はチェックアウトするかメッセージウィンドウを出す）
	// ファイル編集フラグ（Dirty)が付いてるもののみを保存対象にしたいので第一引数はtrue
	// 保存する際に確認のメッセージウィンドウを出さない場合は第二引数をfalseにする
	FEditorFileUtils::PromptForCheckoutAndSave( PackagesToSave, true, true );
 
#endif
}

TObjectPtr<UHumanEquipItemDataTableDataAsset> UHumanEquipItemDataTableDataAsset::GetHumanEquipDataAsset(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::ReturnNull );
	if( World == nullptr )
	{
		return nullptr;
	}
 
	// ゲームインスタンスからStaticDataManagerのインスタンスを取得
	UMasterDataManager* MasterDataManager = Cast<UOriginalGameInstance>( World->GetGameInstance() )->MasterDataManager;
 
	// アセットが読み込まれているかチェック
	if( MasterDataManager->HumanEquipItemDataTableDataAsset == nullptr )
	{
		// iniファイルで指定したデータアセットのファイルパス
		FString DataAssetPath = MasterDataManager->HumanEquipItemDataTableDataAssetPath.ToString();
 
		// アセットを読み込む
		MasterDataManager->HumanEquipItemDataTableDataAsset = Cast<UHumanEquipItemDataTableDataAsset>( StaticLoadObject(StaticClass(), nullptr, *DataAssetPath ));
	}
	return MasterDataManager->HumanEquipItemDataTableDataAsset;
}

void UHumanEquipItemDataTableDataAsset::GetHumanEquipData(const UObject* WorldContextObject, int InID,
	FHumanEquipDataTable& OutHumanEquipData)
{
	UHumanEquipItemDataTableDataAsset* DataAsset = GetHumanEquipDataAsset(WorldContextObject);
	if( DataAsset != nullptr )
	{
		FHumanEquipDataTable* HumanEquipData = DataAsset->HumanEquipDataMap.Find( InID );
		if( HumanEquipData != nullptr )
		{
			OutHumanEquipData = *HumanEquipData;
		}
	}
}

void UHumanEquipItemData::Init(EHumanSlotType InType, const FString& InName)
{
	HumanSlotType = InType;
	Super::Init(EEquipSlotType::Human,InName);
}
