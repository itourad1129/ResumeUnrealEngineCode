// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpRequest.h"
#include "RequestMatch.generated.h"

USTRUCT()
struct FMatchResponse
{
	GENERATED_BODY()

	UPROPERTY()
	int64 UserID = 0;

	UPROPERTY()
	int64 AreaID = 0;

	UPROPERTY()
	int64 LevelID = 0;
};
/**
 * 
 */
UCLASS()
class PJDRC_API URequestMatch : public UHttpRequest
{
	GENERATED_BODY()
public:
	URequestMatch();
	TFuture<TOptional<FMatchResponse>> SendGetRequestAndWait();
	void RequestMatch(TFunction<void(FMatchResponse response)> ResponseCallback);
private:
	static const FString MatchURL;
};
