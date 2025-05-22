// Fill out your copyright notice in the Description page of Project Settings.


#include "Http/RequestLogin.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

const FString URequestLogin::LoginURL = TEXT("login");

URequestLogin::URequestLogin()
{
	
}

TFuture<TOptional<FLoginResponse>> URequestLogin::SendGetRequestAndWait()
{
	// プロミス（非同期処理の結果を格納するコンテナ）を作成
	TSharedPtr<TPromise<TOptional<FLoginResponse>>> Promise = MakeShared<TPromise<TOptional<FLoginResponse>>>();

	// HTTPモジュールのインスタンスを取得
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> HttpRequest = HttpModule.CreateRequest();

	// リクエストの設定
	HttpRequest->SetURL(GetBaseURL() + LoginURL);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	FString JsonPayload = FString::Printf(TEXT("{\"userID\":%lld,\"transferCode\":\"%s\"}"), UserID,*TransferCode);
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
				FLoginResponse ParsedData;
				ParsedData.UserID = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("userID")), nullptr, 10);
				ParsedData.TotalLoginDays = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("totalLoginDays")), nullptr, 10);
				ParsedData.Token = JsonObject->GetStringField(TEXT("token"));
				
				// 成功した場合は結果をプロミスにセット
				Promise->SetValue(ParsedData);
				return;
			}
		}

		// エラーが発生した場合は空をセット
		Promise->SetValue(TOptional<FLoginResponse>());
	});

	// リクエスト送信
	HttpRequest->ProcessRequest();

	// プロミスのFutureを返す
	return Promise->GetFuture();
}

void URequestLogin::RequestLogin(TFunction<void(FLoginResponse response)>ResponseCallback)
{
	TFuture<TOptional<FLoginResponse>> Future = SendGetRequestAndWait();
	AsyncTask(ENamedThreads::AnyThread, [Future = MoveTemp(Future),ResponseCallback,this]() mutable
	{
		TOptional<FLoginResponse> Result = Future.Get();
		if (Result.IsSet())
		{
			AsyncTask(ENamedThreads::GameThread, [Result,ResponseCallback,this]() 
			{
				const FLoginResponse& Data = Result.GetValue();
				UE_LOG(LogTemp, Log, TEXT("UserID: %lld, TotalLoginDays: %lld, Token: %s"), Data.UserID, Data.TotalLoginDays, *Data.Token);
				ResponseCallback(Data);
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, []()
			{
				UE_LOG(LogTemp, Error, TEXT("Login Request Failed or JSON Parsing Error"));
			});
		}
	});
}
