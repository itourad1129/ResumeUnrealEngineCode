// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/OriginalGameModeBase.h"

#include "OriginalGameInstance.h"
#include "GameStateBase/OriginalGameStateBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

AOriginalGameModeBase::AOriginalGameModeBase()
{
	UE_LOG(LogTemp, Log, TEXT("Create Agones"));
	AgonesSDK = CreateDefaultSubobject<UAgonesComponent>(TEXT("AgonesSDK"));
}

void AOriginalGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Log, TEXT("GameMode BeginPlay started"));

	HealthCheckRetryInterval = 5.0f;
	PlayerCountCheckRetryInterval = 60.0f;
	// サーバービルドの場合のみAgones SDKを初期化
#if UE_SERVER
	if (AgonesSDK != nullptr)
	{
		RetryAgonesReady();
	}
#endif
}

void AOriginalGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
#if UE_SERVER
	UE_LOG(LogTemp, Log, TEXT("PostLogin called. Player joined!"));
	auto state = GetWorld()->GetGameState();
	if (state)
	{
		int32 PlayerCount = state->PlayerArray.Num();
		UE_LOG(LogTemp, Log, TEXT("Current Player Count: %d"), PlayerCount);

		// プレイヤー数に応じて Agones に通知
		CheckAndHandlePlayerCount();
	}
#endif
}

void AOriginalGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
#if UE_SERVER
	UE_LOG(LogTemp, Log, TEXT("Logout called. Player left!"));
	auto state = GetWorld()->GetGameState();
	if (state)
	{
		int32 PlayerCount = state->PlayerArray.Num();
		UE_LOG(LogTemp, Log, TEXT("Current Player Count after logout: %d"), PlayerCount);

		// プレイヤー数に応じて Agones に通知
		CheckAndHandlePlayerCount();
	}
#endif
}

void AOriginalGameModeBase::CheckAndHandlePlayerCount()
{
	AOriginalGameStateBase* originalGameStateBase = GetGameState<AOriginalGameStateBase>();
	if (!originalGameStateBase)
	{
		return;
	}

	int32 PlayerCount = originalGameStateBase->PlayerArray.Num();
    
	if (PlayerCount <= 0 && IsPlayerLogin)
	{
		UE_LOG(LogTemp, Warning, TEXT("No players left. Triggering Agones Shutdown."));
		if (AgonesSDK)
		{
			FShutdownDelegate OnSuccess;
			OnSuccess.BindUFunction(this, FName("OnAgonesShutdownSuccess"));

			FAgonesErrorDelegate OnError;
			OnError.BindUFunction(this, FName("OnAgonesShutdownError"));
			AgonesSDK->Shutdown(OnSuccess,OnError);
		}
	}

	if (1 <= PlayerCount && !IsPlayerLogin)
	{
		IsPlayerLogin = true;
		UE_LOG(LogTemp, Log, TEXT("First player joined. Allocating GameServer."));
		if (AgonesSDK)
		{
			FAllocateDelegate OnSuccess;
			OnSuccess.BindUFunction(this, FName("OnAgonesAllocateSuccess"));

			FAgonesErrorDelegate OnError;
			OnError.BindUFunction(this, FName("OnAgonesAllocateError"));
			AgonesSDK->Allocate(OnSuccess,OnError);
		}
	}
}

void AOriginalGameModeBase::OnAgonesShutdownSuccess()
{
	UE_LOG(LogTemp, Log, TEXT("Agones Shutdown successful!"));
}

void AOriginalGameModeBase::OnAgonesShutdownError()
{
	UE_LOG(LogTemp, Log, TEXT("Agones Shutdown failed"));
}

void AOriginalGameModeBase::OnAgonesAllocateSuccess()
{
	UE_LOG(LogTemp, Log, TEXT("Agones Allocate successful!"));
}

void AOriginalGameModeBase::OnAgonesAllocateError()
{
	UE_LOG(LogTemp, Log, TEXT("Agones Allocate failed"));
}

void AOriginalGameModeBase::RetryAgonesReady()
{
	if (AgonesSDK)
	{
		UE_LOG(LogTemp, Log, TEXT("Trying to set Agones Ready..."));

		FReadyDelegate OnSuccess;
		OnSuccess.BindUFunction(this, FName("OnAgonesReadySuccess"));

		FAgonesErrorDelegate OnError;
		OnError.BindUFunction(this, FName("OnAgonesReadyError"));

		AgonesSDK->Ready(OnSuccess, OnError);
	}
}

void AOriginalGameModeBase::OnAgonesReadySuccess()
{
	UE_LOG(LogTemp, Log, TEXT("AgonesComponent is ready to accept connections."));

	// GameServer情報を取得
	if (AgonesSDK != nullptr)
	{
		FGameServerDelegate OnSuccess;
		OnSuccess.BindUFunction(this, FName("OnGameServerInfoReceived"));

		FAgonesErrorDelegate OnError;
		OnError.BindUFunction(this, FName("OnGameServerInfoError"));

		AgonesSDK->GameServer(OnSuccess, OnError);
		if (GetWorld())
		{
			// **リトライ用タイマーを停止**
			GetWorld()->GetTimerManager().ClearTimer(ReadyRetryTimerHandle);
			// **ヘルスチェックを一定間隔で呼び出す**
			GetWorld()->GetTimerManager().SetTimer(HealthCheckTimerHandle, this, &AOriginalGameModeBase::SendHealthPing, HealthCheckRetryInterval, true);
			// **プレイヤー数チェックを一定間隔で呼び出す**
			GetWorld()->GetTimerManager().SetTimer(PlayerCountCheckTimerHandle, this, &AOriginalGameModeBase::CheckAndHandlePlayerCount, PlayerCountCheckRetryInterval, true);
		}
	}
}

void AOriginalGameModeBase::OnGameServerInfoReceived(const FGameServerResponse& GameServer)
{
	// GameServerからIPアドレスとポートを取得してログに出力
	FString IPAddress = GameServer.Status.Address;
	int32 Port = GameServer.Status.Ports[0].Port;

	FString ServerAddress = IPAddress + FString(":") + FString::FromInt(Port);
	
	UE_LOG(LogTemp, Log, TEXT("GameServer Info Received: IP: %s, Port: %d"), *IPAddress, Port);

	//SendHealthPing();

	// 必要に応じて接続情報を使用（例: クライアントへの通知やログ出力）
}

void AOriginalGameModeBase::OnGameServerInfoError(const FString& ErrorMessage)
{
	UE_LOG(LogTemp, Error, TEXT("Failed to retrieve GameServer Info: %s"), *ErrorMessage);
}

void AOriginalGameModeBase::OnAgonesReadyError()
{
	UE_LOG(LogTemp, Error, TEXT("AgonesComponent failed to mark Ready, retrying..."));
	if (GetWorld())
	{
		// **一定時間後に再度 Ready を試行**
		GetWorld()->GetTimerManager().SetTimer(ReadyRetryTimerHandle, this, &AOriginalGameModeBase::RetryAgonesReady, HealthCheckRetryInterval, false);
	}
}

void AOriginalGameModeBase::SendHealthPing()
{
	if (AgonesSDK != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Health check start"));
		// 成功時のデリゲート
		FHealthDelegate OnHealthSuccess;
		OnHealthSuccess.BindUFunction(this, FName("OnHealthSuccess"));

		// 失敗時のデリゲート
		FAgonesErrorDelegate OnHealthError;
		OnHealthError.BindUFunction(this, FName("OnHealthError"));
		AgonesSDK->Health(OnHealthSuccess, OnHealthError);
	}
}

void AOriginalGameModeBase::OnHealthSuccess()
{
	UE_LOG(LogTemp, Log, TEXT("Health check successful!"));
}

void AOriginalGameModeBase::OnHealthError(const FString& ErrorMessage)
{
	UE_LOG(LogTemp, Error, TEXT("Health check failed: %s"), *ErrorMessage);
}

// Called every frame
// void AOriginalGameModeBase::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 	UE_LOG(LogTemp, Log, TEXT("Tick................"));
// #if UE_SERVER //サーバー定期実行処理開始
// 	TimeSinceLastRetry += DeltaTime;
// 	if (AgonesSDK != nullptr)
// 	{
// 		// リトライの間隔が経過したら再度 Ready を呼び出す
// 		if (TimeSinceLastRetry >= RetryInterval)
// 		{
// 			TimeSinceLastRetry = 0.0f;
// 			if (bIsRetrying)
// 			{
// 				UE_LOG(LogTemp, Log, TEXT("Retrying Agones ready state..."));
// 			
// 				// 成功/失敗時のデリゲートバインドを再度呼び出し
// 				FReadyDelegate OnSuccess;
// 				OnSuccess.BindUFunction(this, FName("OnAgonesReadySuccess"));
//
// 				FAgonesErrorDelegate OnError;
// 				OnError.BindUFunction(this, FName("OnAgonesReadyError"));
//
// 				// 再度 AgonesSDK の Ready を呼び出す
// 				AgonesSDK->Ready(OnSuccess, OnError);
// 			}
// 			SendHealthPing();
// 		}
// 	}
// #endif //サーバー定期実行処理終了 WITH_SERVER_CODE
// }