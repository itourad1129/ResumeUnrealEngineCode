// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpRequest.h"
#include "RequestChunkVersion.generated.h"

/**
 * 
 */

USTRUCT()
struct FChunkVersionData
{
    GENERATED_BODY()

    UPROPERTY()
    int64 VersionID = 0;

	UPROPERTY()
	int64 PlatformType = 0;

	UPROPERTY()
	FString DeploymentName = "";

    UPROPERTY()
    FString ContentBuildID = "";
};

UCLASS()
class PJDRC_API URequestChunkVersion : public UHttpRequest
{
	GENERATED_BODY()
public:
	URequestChunkVersion();

	TFuture<TOptional<FChunkVersionData>> SendGetRequestAndWait();
	void RequestChunkVersion(TFunction<void(FChunkVersionData response)>ResponseCallback);

private:
	static const FString ChunkVersionURL;
	FString PlatformType;
};
