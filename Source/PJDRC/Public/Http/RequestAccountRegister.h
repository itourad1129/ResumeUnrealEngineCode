// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpRequest.h"
#include "RequestAccountRegister.generated.h"

/**
 * 
 */

USTRUCT()
struct FAccountRegisterResponse
{
	GENERATED_BODY()

	UPROPERTY()
	int64 UserID = 0;

	UPROPERTY()
	FString TransferCode = "";
};

UCLASS()
class PJDRC_API URequestAccountRegister : public UHttpRequest
{
	GENERATED_BODY()
public:
	URequestAccountRegister();

	TFuture<TOptional<FAccountRegisterResponse>> SendGetRequestAndWait();
	void RequestAccountRegister(TFunction<void(FAccountRegisterResponse response)> ResponseCallback);

private:
	FText DefaultPlayerName;
	static const FString AccountRegisterURL;
};
