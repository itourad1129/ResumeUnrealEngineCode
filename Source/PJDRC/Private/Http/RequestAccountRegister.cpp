// Fill out your copyright notice in the Description page of Project Settings.


#include "Http/RequestAccountRegister.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

const FString URequestAccountRegister::AccountRegisterURL = TEXT("userRegister");

URequestAccountRegister::URequestAccountRegister()
{
	DefaultPlayerName = NSLOCTEXT("TitleScreen", "DefaultUserName", "Adventurer");
}

TFuture<TOptional<FAccountRegisterResponse>> URequestAccountRegister::SendGetRequestAndWait()
{
	// プロミス（非同期処理の結果を格納するコンテナ）を作成
	TSharedPtr<TPromise<TOptional<FAccountRegisterResponse>>> Promise = MakeShared<TPromise<TOptional<FAccountRegisterResponse>>>();

	// HTTPモジュールのインスタンスを取得
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> HttpRequest = HttpModule.CreateRequest();

	// リクエストの設定
	HttpRequest->SetURL(GetBaseURL() + AccountRegisterURL);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	FString JsonPayload = FString::Printf(TEXT("{\"name\":\"%s\"}"), *DefaultPlayerName.ToString());
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
				FAccountRegisterResponse ParsedData;
				ParsedData.UserID = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("userID")), nullptr, 10);
				ParsedData.TransferCode = JsonObject->GetStringField(TEXT("transferCode"));
				
				// 成功した場合は結果をプロミスにセット
				Promise->SetValue(ParsedData);
				return;
			}
		}

		// エラーが発生した場合は空をセット
		Promise->SetValue(TOptional<FAccountRegisterResponse>());
	});

	// リクエスト送信
	HttpRequest->ProcessRequest();

	// プロミスのFutureを返す
	return Promise->GetFuture();
}

void URequestAccountRegister::RequestAccountRegister(TFunction<void(FAccountRegisterResponse response)>ResponseCallback)
{
	TFuture<TOptional<FAccountRegisterResponse>> Future = SendGetRequestAndWait();
	AsyncTask(ENamedThreads::AnyThread, [Future = MoveTemp(Future),ResponseCallback,this]() mutable
	{
		TOptional<FAccountRegisterResponse> Result = Future.Get();
		if (Result.IsSet())
		{
			AsyncTask(ENamedThreads::GameThread, [Result,ResponseCallback,this]() 
			{
				const FAccountRegisterResponse& Data = Result.GetValue();
				UE_LOG(LogTemp, Log, TEXT("UserID: %lld, TransferCode): %s"), Data.UserID, *Data.TransferCode);
				if (Data.TransferCode != "" && Data.UserID != 0)
				{
					ResponseCallback(Data);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UserID,TransferCode Not Found"));
				}
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, []()
			{
				UE_LOG(LogTemp, Error, TEXT("Account Register Request Failed or JSON Parsing Error"));
			});
		}
	});
}
