// Fill out your copyright notice in the Description page of Project Settings.

#include "Http/RequestChunkVersion.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

const FString URequestChunkVersion::ChunkVersionURL = TEXT("getChunkVersion");

URequestChunkVersion::URequestChunkVersion()
{
#if PLATFORM_MAC
	PlatformType = "1";
#elif PLATFORM_WINDOWS
	PlatformType = "2";
#elif PLATFORM_IOS
	PlatformType = "3";
#elif PLATFORM_ANDROID
	PlatformType = "4";
#else
	PlatformType = "0";
#endif
}

// HTTPリクエストを送信して、非同期でレスポンスを待つ
TFuture<TOptional<FChunkVersionData>> URequestChunkVersion::SendGetRequestAndWait()
{
	// プロミス（非同期処理の結果を格納するコンテナ）を作成
	TSharedPtr<TPromise<TOptional<FChunkVersionData>>> Promise = MakeShared<TPromise<TOptional<FChunkVersionData>>>();

	// HTTPモジュールのインスタンスを取得
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> HttpRequest = HttpModule.CreateRequest();

	// リクエストの設定
	HttpRequest->SetURL(GetBaseURL() + ChunkVersionURL);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	FString JsonPayload = FString::Printf(TEXT("{\"platformType\":%s}"), *PlatformType);
	HttpRequest->SetContentAsString(JsonPayload);

	// リクエストの完了コールバックを設定
	HttpRequest->OnProcessRequestComplete().BindLambda([Promise](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			FString ResponseString = Response->GetContentAsString();
			
			// JSONデコード
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				FChunkVersionData ParsedData;
				ParsedData.VersionID = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("versionID")), nullptr, 10);
				ParsedData.PlatformType = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("platformType")), nullptr, 10);
				ParsedData.DeploymentName = JsonObject->GetStringField(TEXT("deploymentName"));
				ParsedData.ContentBuildID = JsonObject->GetStringField(TEXT("contentBuildID"));
				
				// 成功した場合は結果をプロミスにセット
				Promise->SetValue(ParsedData);
				return;
			}
		}

		// エラーが発生した場合は空をセット
		Promise->SetValue(TOptional<FChunkVersionData>());
	});

	// リクエスト送信
	HttpRequest->ProcessRequest();

	// プロミスのFutureを返す
	return Promise->GetFuture();
}

void URequestChunkVersion::RequestChunkVersion(TFunction<void(FChunkVersionData response)>ResponseCallback)
{
	TFuture<TOptional<FChunkVersionData>> Future = SendGetRequestAndWait();
	AsyncTask(ENamedThreads::AnyThread, [Future = MoveTemp(Future),ResponseCallback,this]() mutable
	{
		TOptional<FChunkVersionData> Result = Future.Get();
		if (Result.IsSet())
		{
			AsyncTask(ENamedThreads::GameThread, [Result,ResponseCallback,this]() 
			{
				const FChunkVersionData& Data = Result.GetValue();
				UE_LOG(LogTemp, Log, TEXT("VersionID: %d, DeploymentName: %s"), Data.VersionID, *Data.DeploymentName);
				ResponseCallback(Data);
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, []()
			{
				UE_LOG(LogTemp, Error, TEXT("Chunk Version Request Failed or JSON Parsing Error"));
			});
		}
	});
}