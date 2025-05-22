

#pragma once

#include "CoreMinimal.h"
#include "HttpRequest.h"
#include "RequestMasterDataVersion.generated.h"

/**
 * 
 */

USTRUCT()
struct FMasterDataVersion
{
	GENERATED_BODY()

	UPROPERTY()
	int64 MasterDataID = 0;

	UPROPERTY()
	int64 Version = 0;

	UPROPERTY()
	int64 ChunkID = 0;
};

UCLASS()
class PJDRC_API URequestMasterDataVersion : public UHttpRequest
{
	GENERATED_BODY()
public:
	URequestMasterDataVersion();

	TFuture<TArray<FMasterDataVersion>> SendGetRequestAndWait();
	void RequestMasterDataVersion(TFunction<void(TArray<FMasterDataVersion>)>ResponseCallback);
private:
	static const FString MasterDataVersionURL;
};
