#include "Http/RequestMasterDataVersion.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

const FString URequestMasterDataVersion::MasterDataVersionURL = TEXT("getMasterDataVersion");

URequestMasterDataVersion::URequestMasterDataVersion()
{
	
}

TFuture<TArray<FMasterDataVersion>> URequestMasterDataVersion::SendGetRequestAndWait()
{
	TSharedPtr<TPromise<TArray<FMasterDataVersion>>> Promise = MakeShared<TPromise<TArray<FMasterDataVersion>>>();
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> HttpRequest = HttpModule.CreateRequest();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetBaseURL());
	HttpRequest->SetURL(GetBaseURL() + MasterDataVersionURL);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	HttpRequest->OnProcessRequestComplete().BindLambda([Promise](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		TArray<FMasterDataVersion> ParsedDataArray;
		if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			FString ResponseString = Response->GetContentAsString();
			TArray<TSharedPtr<FJsonValue>> JsonArray;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
			if (FJsonSerializer::Deserialize(Reader, JsonArray) && JsonArray.Num() > 0)
			{
				for (TSharedPtr<FJsonValue> Value : JsonArray)
				{
					TSharedPtr<FJsonObject> JsonObject = Value->AsObject();
					if (JsonObject.IsValid())
					{
						FMasterDataVersion ParsedData;
						ParsedData.MasterDataID = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("masterDataID")), nullptr, 10);
						ParsedData.Version = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("version")),nullptr, 10);
						ParsedData.ChunkID = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("chunkID")), nullptr, 10);
						ParsedDataArray.Add(ParsedData);
					}
				}
				Promise->SetValue(ParsedDataArray);
				return;
			}
		}
		Promise->SetValue(TArray<FMasterDataVersion>());
	});
	HttpRequest->ProcessRequest();
	return Promise->GetFuture();
}

void URequestMasterDataVersion::RequestMasterDataVersion(TFunction<void(TArray<FMasterDataVersion>)>ResponseCallback)
{
	TFuture<TArray<FMasterDataVersion>> Future = SendGetRequestAndWait();
	AsyncTask(ENamedThreads::AnyThread, [Future = MoveTemp(Future),ResponseCallback,this]() mutable
	{
		TArray<FMasterDataVersion> Result = Future.Get();
		if (Result.Num() > 0)
		{
			AsyncTask(ENamedThreads::GameThread, [Result, ResponseCallback, this]() 
			{
				ResponseCallback(Result);
			});
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Master Data Version Request Failed or JSON Parsing Error"));
		}
	});
}
