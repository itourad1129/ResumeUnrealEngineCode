// Fill out your copyright notice in the Description page of Project Settings.


#include "Http/RequestMatch.h"
#include "HttpModule.h"
#include "GameFramework/SaveGame.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "User/UserSaveGame.h"

const FString URequestMatch::MatchURL = TEXT("matchRequest");

URequestMatch::URequestMatch()
{
	
}

TFuture<TOptional<FMatchResponse>> URequestMatch::SendGetRequestAndWait()
{
	// プロミス（非同期処理の結果を格納するコンテナ）を作成
	TSharedPtr<TPromise<TOptional<FMatchResponse>>> Promise = MakeShared<TPromise<TOptional<FMatchResponse>>>();

	// HTTPモジュールのインスタンスを取得
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> HttpRequest = HttpModule.CreateRequest();

	FString Token = "";
	USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(TEXT("UserSaveSlot"), 0);
	UUserSaveGame* LoadedSave = Cast<UUserSaveGame>(LoadedGame);
	if (LoadedSave)
	{
		Token = LoadedSave->Token;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast loaded save game to LoadedSave->Token."));
	}

	// リクエストの設定
	HttpRequest->SetURL(GetBaseURL() + "auth/" + MatchURL);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Token));

	// リクエストの完了コールバックを設定
	HttpRequest->OnProcessRequestComplete().BindLambda([Promise](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			FString ResponseString = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *ResponseString);
			// JSONデコード
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				FMatchResponse ParsedData;
				ParsedData.UserID = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("userID")), nullptr, 10);
				ParsedData.AreaID = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("areaID")), nullptr, 10);
				ParsedData.LevelID = FCString::Strtoi64(*JsonObject->GetStringField(TEXT("levelID")), nullptr, 10);
				// 成功した場合は結果をプロミスにセット
				Promise->SetValue(ParsedData);
				return;
			}
		}

		// エラーが発生した場合は空をセット
		Promise->SetValue(TOptional<FMatchResponse>());
	});

	// リクエスト送信
	HttpRequest->ProcessRequest();

	// プロミスのFutureを返す
	return Promise->GetFuture();
}

void URequestMatch::RequestMatch(TFunction<void(FMatchResponse response)>ResponseCallback)
{
	TFuture<TOptional<FMatchResponse>> Future = SendGetRequestAndWait();
	AsyncTask(ENamedThreads::AnyThread, [Future = MoveTemp(Future),ResponseCallback,this]() mutable
	{
		TOptional<FMatchResponse> Result = Future.Get();
		if (Result.IsSet())
		{
			AsyncTask(ENamedThreads::GameThread, [Result,ResponseCallback,this]() 
			{
				const FMatchResponse& Data = Result.GetValue();
				UE_LOG(LogTemp, Log, TEXT("UserID: %lld, AreaID: %lld, LevelID: %lld"), Data.UserID, Data.AreaID, Data.LevelID);
				ResponseCallback(Data);
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, []()
			{
				UE_LOG(LogTemp, Error, TEXT("Match Request Failed or JSON Parsing Error"));
			});
		}
	});
}
