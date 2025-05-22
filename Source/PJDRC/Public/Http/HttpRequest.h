// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UObject/Object.h"
#include "HttpRequest.generated.h"

// デリゲートの宣言
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHttpResponseReceived, const FString&, ResponseContent, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class PJDRC_API UHttpRequest : public UObject
{
	GENERATED_BODY()
public:
	UHttpRequest();

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void SendGetRequest(const FString& URL);

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void SendPostRequest(const FString& URL, const FString& Content);
	
	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FOnHttpResponseReceived OnHttpResponseReceived;

private:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
protected:
	FString GetBaseURL();
};
