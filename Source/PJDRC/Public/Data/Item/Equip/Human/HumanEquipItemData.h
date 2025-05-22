// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/Equip/EquipItemData.h"
#include "UObject/Object.h"
#include "HumanEquipItemData.generated.h"

/**
 * 
 */

// データテーブル用構造体
USTRUCT()
struct FHumanEquipDataTableRowBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
 
	UPROPERTY( EditAnywhere )
	int ID = 0;
	UPROPERTY( EditAnywhere )
	int SlotType = 0;
	UPROPERTY( EditAnywhere )
	FString Name = "";
	UPROPERTY( EditAnywhere )
	int ChunkID = 0;
};
 
// 実際にゲーム側で使うキャラクターデータの構造体
USTRUCT( BlueprintType )
struct FHumanEquipDataTable
{
	GENERATED_USTRUCT_BODY()
 
	UPROPERTY( BlueprintReadOnly,  EditAnywhere )
	int ID = 0;
	UPROPERTY( BlueprintReadOnly,  EditAnywhere )
	int SlotType = 0;
	UPROPERTY( BlueprintReadOnly,  EditAnywhere )
	FString Name = "";
	UPROPERTY( BlueprintReadOnly,  EditAnywhere )
	int ChunkID = 0;
};

// データアセット用クラス
UCLASS()
class PJDRC_API UHumanEquipItemDataTableDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
 
#if WITH_EDITORONLY_DATA
	UPROPERTY( BlueprintReadOnly, EditAnywhere )
	TObjectPtr<UDataTable> DataTable;
#endif
 
	// キャラクターデータ（TArrayだったりTMapだったりのデータ配列）
	UPROPERTY( BlueprintReadOnly, VisibleAnywhere )
	TMap< int, FHumanEquipDataTable > HumanEquipDataMap;
 
	// データ作成用の関数
	UFUNCTION( BlueprintCallable, meta = (CallInEditor = "true") )
	void Build();

	// データアセットの取得
	//UFUNCTION( BlueprintCallable, meta = (WorldContext = "WorldContextObject") )
	static TObjectPtr<UHumanEquipItemDataTableDataAsset> GetHumanEquipDataAsset( const UObject* WorldContextObject );
 
	//任意のキャラクターデータを取得（※本来はキャラクターを管理する別クラスで実装すべき）
	UFUNCTION( BlueprintCallable, meta = (WorldContext = "WorldContextObject") )
	static void GetHumanEquipData( const UObject* WorldContextObject, int InID, FHumanEquipDataTable& OutHumanEquipData );
};

UENUM(BlueprintType)
enum class EHumanSlotType : uint8
{
	Head UMETA(DisplayName = "Head"),
	Body UMETA(DisplayName = "Body"),
	Hand UMETA(DisplayName = "Hand"),
	Pants UMETA(DisplayName = "Pants"),
	Boots UMETA(DisplayName = "Boots"),
	Gear_1 UMETA(DisplayName = "Gear_1"),
	Gear_2 UMETA(DisplayName = "Gear_2"),
};
UENUM(BlueprintType)
enum class EHumanOwnerType : uint8
{
	None UMETA(DisplayName = "None"),
	NPC UMETA(DisplayName = "NPC"),
	Player UMETA(DisplayName = "Player"),
};
UCLASS()
class PJDRC_API UHumanEquipItemData : public UEquipItemData
{
	GENERATED_BODY()
protected:
	using UEquipItemData::Init;
	virtual void Init(EHumanSlotType InType,const FString& InName);
	EHumanSlotType HumanSlotType;
	EHumanOwnerType HumanOwnerType;
private:
};
