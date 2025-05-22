// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Asset/Item/Equip/EquipItemAssetData.h"

#include "OriginalGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

void UEquipItemAssetData::SetEquipAssetData(UOriginalGameInstance* GameIns, 
										USkeletalMeshComponent* SkMeshCmp, 
										TSoftObjectPtr<USkeletalMesh> SkMesh, 
										int32 ChunkID)
{
	TFunction<void(void)> Callback = [this, SkMeshCmp, SkMesh]()
	{
		UKismetSystemLibrary::PrintString(this, "patchGameTask Success3", true, true, FColor::Cyan, 30.f, TEXT("None"));
        
		// スケルタルメッシュをロード
		USkeletalMesh* LoadSkeletalMesh = Cast<USkeletalMesh>(SkMesh.Get());
		if (LoadSkeletalMesh == nullptr)
		{
			LoadSkeletalMesh = SkMesh.LoadSynchronous();
		}
        
		// メッシュコンポーネントに設定
		if (SkMeshCmp != nullptr)
		{
			SkMeshCmp->SetSkeletalMesh(LoadSkeletalMesh);
            
			// アニメーションクラスの設定
			if (ACharacter* CharacterOwner = Cast<ACharacter>(SkMeshCmp->GetOwner()))
			{
				SkMeshCmp->SetAnimClass(CharacterOwner->GetMesh()->AnimClass);
			}
		}
	};

	// チャンクのダウンロードタスクを登録
	if (GameIns && GameIns->Chunk)
	{
		GameIns->Chunk->EnqueueChunkDownloadTask(ChunkID, Callback);
	}
}
