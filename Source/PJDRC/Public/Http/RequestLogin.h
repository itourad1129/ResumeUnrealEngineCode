// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpRequest.h"
#include "RequestLogin.generated.h"

USTRUCT()
struct FLoginResponse
{
	GENERATED_BODY()

	UPROPERTY()
	int64 UserID = 0;

	UPROPERTY()
	int64 TotalLoginDays = 0;

	UPROPERTY()
	FString Token = "";
};
/**
 * 
 */
UCLASS()
class PJDRC_API URequestLogin : public UHttpRequest
{
	GENERATED_BODY()
public:
	URequestLogin();
	TFuture<TOptional<FLoginResponse>> SendGetRequestAndWait();
	void RequestLogin(TFunction<void(FLoginResponse response)> ResponseCallback);
	int64 UserID = 0;
	FString TransferCode = "";
private:
	static const FString LoginURL;
};
