// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AgonesComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OriginalGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API AOriginalGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOriginalGameModeBase();
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAgonesComponent* AgonesSDK;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
private:
	bool IsPlayerLogin = false;
	float HealthCheckRetryInterval; // ヘルスチェックリトライ間隔
	float PlayerCountCheckRetryInterval; //プレイヤー数確認リトライ間隔
	FTimerHandle HealthCheckTimerHandle;
	FTimerHandle PlayerCountCheckTimerHandle;
	FTimerHandle ReadyRetryTimerHandle;
	UFUNCTION()
	void OnAgonesReadySuccess();
	UFUNCTION()
	void OnGameServerInfoReceived(const FGameServerResponse& GameServer);
	UFUNCTION()
	void OnGameServerInfoError(const FString& ErrorMessage);
	UFUNCTION()
	void OnAgonesReadyError();
	void SendHealthPing();
	UFUNCTION()
	void OnHealthSuccess();
	UFUNCTION()
	void OnHealthError(const FString& ErrorMessage);
	UFUNCTION()
	void RetryAgonesReady();
	void CheckAndHandlePlayerCount();
	void OnAgonesShutdownSuccess();
	void OnAgonesShutdownError();
	void OnAgonesAllocateSuccess();
	void OnAgonesAllocateError();
};
