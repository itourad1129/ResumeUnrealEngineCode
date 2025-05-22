// Fill out your copyright notice in the Description page of Project Settings.


#include "OriginalGameInstance.h"
#include "Data/StaticDataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/CoreDelegates.h"

void UOriginalGameInstance::Init()
{
	Super::Init();
#if !UE_SERVER //spannerエミュレーター環境構築まで一旦サーバー実行禁止
	MasterDataManager = NewObject<UMasterDataManager>( this );
	MasterDataManager->Init();
	
	Chunk = NewObject<UChunk>( this );
	//if (FParse::Value(FCommandLine::Get(), TEXT("-serverip="), ServerIP))
	//{
		// サーバーへの接続処理
	//}
#endif
	//LoadLevel(this->GetWorld());
}

void UOriginalGameInstance::Shutdown()
{
	Super::Shutdown();
	// Shut down ChunkDownloader
	if(Chunk->Visible)
	{
		Chunk->ShutDown();
	}
}

//void UOriginalGameInstance::LoadLevel(TSoftObjectPtr<UWorld> World)
//{
// 	UE_LOG(LogTemp, Warning, TEXT("Start OpenLevel:%p"), World.LoadSynchronous());
// 	//Level移動
// #if UE_SERVER
// 	UGameplayStatics::OpenLevel(GetWorld(), "MultiPlayerMap",false,"listen");
// #else
// 	UGameplayStatics::OpenLevel(GetWorld(), "MultiPlayerMap",false,"127.0.0.1:7777");
// 	//UGameplayStatics::OpenLevel(GetWorld(),"MultiPlayerMap",false);
// 	//GetWorld()->SeamlessTravel("127.0.0.1:7777/Game/OriginalContent/Maps/MultiPlayerMap", false);
// #endif
// 	//Level移動後に呼ばれるイベントを登録
// 	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UOriginalGameInstance::LoadedEvent);
//}

void UOriginalGameInstance::LoadedEvent(UWorld* LoadedWorld)
{
	//移動先のLevel名をLogに出す
	UE_LOG(LogTemp, Warning, TEXT("LoadedWorld: %s"), *LoadedWorld->GetName());
}

void UOriginalGameInstance::ConnectServer(FString ServerAddress)
{
	if(PlayerSpawned == false)
	{
		PlayerSpawned = true;
		//FString ServerIP = TEXT("127.0.0.1");

		FURL URL;
		URL.Host = TEXT("172.17.168.126");  // サーバーのIPアドレス
		URL.Port = 7939;
		
		this->GetWorld()->GetFirstPlayerController()->ClientTravel(URL.ToString(), TRAVEL_Absolute);
	}
}
