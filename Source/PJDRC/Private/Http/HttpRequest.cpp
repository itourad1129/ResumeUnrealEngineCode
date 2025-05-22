// Fill out your copyright notice in the Description page of Project Settings.


#include "Http/HttpRequest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Logging/LogMacros.h"

UHttpRequest::UHttpRequest()
{
}

void UHttpRequest::SendGetRequest(const FString& URL)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->SetHeader("Content-Type", "application/json");
	Request->OnProcessRequestComplete().BindUObject(this, &UHttpRequest::OnResponseReceived);
	Request->ProcessRequest();
}

void UHttpRequest::SendPostRequest(const FString& URL, const FString& Content)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindUObject(this, &UHttpRequest::OnResponseReceived);
	Request->ProcessRequest();
}

void UHttpRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString ResponseContent = Response.IsValid() ? Response->GetContentAsString() : TEXT("");
	
	OnHttpResponseReceived.Broadcast(ResponseContent, bWasSuccessful);
}

FString UHttpRequest::GetBaseURL()
{
	FString BaseUrl;
	if (GConfig->GetString(TEXT("/Script/PJDRC.HttpRequest"), TEXT("ApiBaseUrl"), BaseUrl, GGameIni))
	{
		return BaseUrl;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load ApiBaseUrl from config!"));
	}
	return TEXT("");
}