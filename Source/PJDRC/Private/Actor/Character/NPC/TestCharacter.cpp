// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/NPC/TestCharacter.h"

#include "OriginalGameInstance.h"
#include "Data/Asset/Item/Equip/Human/HumanEquipPAL.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/OriginalAssetManager.h"


// Sets default values
ATestCharacter::ATestCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EquipHairSkComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipHairSkComp"));
	EquipHairSkComp->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// UGameInstance* gameInstance = GetGameInstance();
	// if(gameInstance != nullptr && Cast<UOriginalGameInstance>(gameInstance))
	// {
	// 	UOriginalGameInstance* oGameIns = Cast<UOriginalGameInstance>(gameInstance);
	// 	if(!isPatchGameSuccess)
	// 	{
	// 		isPatchGameSuccess = oGameIns->PatchGame(999);
	// 	}
	// 	if(oGameIns->CheckMountComplete(999) && !isHairLoaded)
	// 	{
	// 		isHairLoaded = true;
	// 		LoadSpecificSwordMeshAsync([this](USkeletalMesh* LoadedMesh)
	// 			{
	// 				if (LoadedMesh)
	// 				{
	// 					EquipHairSkComp->SetSkeletalMesh(LoadedMesh);
	// 					UKismetSystemLibrary::PrintString(this,"Successfully loaded", true, true, FColor::Cyan, 30.f, TEXT("None"));
	// 				}
	// 			}
	// 		);
	// 	}
	// }
}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATestCharacter::LoadSpecificSwordMeshAsync(TFunction<void(USkeletalMesh*)> OnMeshLoaded)
{
	// プリロードを開始します
	UOriginalAssetManager& AssetManager = UOriginalAssetManager::GetAssetManager();
	//AssetManager.LoadPrimaryAsset(FPrimaryAssetId("HumanEquipPAL:EquipHuman_Head_0"));
	FPrimaryAssetId Pid = FPrimaryAssetId("HumanEquipPAL:TestDataAsset");
	// アセットのロードを非同期で行うためのハンドルを取得
	TSharedPtr<FStreamableHandle> Handle = AssetManager.LoadPrimaryAsset(Pid, TArray<FName>(), FStreamableDelegate::CreateLambda([Pid,OnMeshLoaded]()
	{
		// ロードが完了したときのコールバック
		UE_LOG(LogTemp, Log, TEXT("Successfully loaded asset: %s"), *Pid.ToString());
		// 必要な処理をここで行う
		UAssetManager& AssetManager = UAssetManager::Get();
		UHumanEquipPAL* DataAsset = Cast<UHumanEquipPAL>(AssetManager.GetPrimaryAssetObject(Pid));

		if (DataAsset)
		{
			UHumanEquipPAL* SwordDataAsset = Cast<UHumanEquipPAL>(DataAsset);
			UObject* ret = SwordDataAsset->ExplicitAssets[0].Get();
			if(ret == nullptr)
			{
				ret = SwordDataAsset->ExplicitAssets[0].LoadSynchronous();
			}
			USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(ret);
			if(SkeletalMesh)
			{
				// メッシュがロードされた後のコールバックを実行
				OnMeshLoaded(SkeletalMesh);	
			}
		}
	}));
	// if (Handle.IsValid() && Handle->HasLoadCompleted())
	// {
	// 	Handle->BindCompleteDelegate(FStreamableDelegate::CreateLambda([Pid,this]()
	// 	{
	// 		// ロードが完了したときのコールバック
	// 		UE_LOG(LogTemp, Log, TEXT("Successfully loaded asset: %s"), *Pid.ToString());
	// 		UKismetSystemLibrary::PrintString(this,"Successfully loaded asset", true, true, FColor::Cyan, 30.f, TEXT("None"));
	// 		// 必要な処理をここで行う
	// 	}));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Failed to load asset: %s"), *Pid.ToString());
	// 	UKismetSystemLibrary::PrintString(this,"Failed to load asset", true, true, FColor::Cyan, 30.f, TEXT("None"));
	// }
}

