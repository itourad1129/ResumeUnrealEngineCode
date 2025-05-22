// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/TitleGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "User/UserSaveGame.h"
#include "Widget/Title/TitleScreenWidget.h"
#include "Components/TextBlock.h"
#include "Http/RequestAccountRegister.h"
#include "Http/RequestLogin.h"
#include "Http/RequestMatch.h"

void ATitleGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Init_TitleScreen();
	
}

void ATitleGameModeBase::Init_TitleScreen()
{
	if (TitleScreenWidgetClass.ToSoftObjectPath().IsValid())
	{
		TitleScreenWidgetClass.LoadSynchronous();
		TitleScreenWidget = CreateWidget<UTitleScreenWidget>(GetWorld(), TitleScreenWidgetClass.Get());
		if (TitleScreenWidget)
		{
			TitleScreenWidget->AddToViewport();
			if (false == UGameplayStatics::DoesSaveGameExist(TEXT("UserSaveSlot"), 0))
			{
				IsNewGame = true;
				TitleScreenWidget->StartButtonText->SetText(NSLOCTEXT("TitleScreen", "StartButton", "New Game"));
			}
			
			TitleScreenWidget->StartButton->OnClicked.AddDynamic(this, &ATitleGameModeBase::OnStartButtonClicked);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TitleScreenWidgetClass is not valid!"));
	}
}

void ATitleGameModeBase::OnStartButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Startボタンが押されました！"));
	if (IsNewGame) //ニューゲーム(アカウント登録へ)
	{
		//アカウント登録後、ログイン
		TFunction<void(FLoginResponse response)> LoginResponseCallback = [this](FLoginResponse response)
		{
			USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(TEXT("UserSaveSlot"), 0);
			UUserSaveGame* SaveGameInstance = Cast<UUserSaveGame>(LoadedGame);
			SaveGameInstance->Token = response.Token;
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("UserSaveSlot"), 0);
			UE_LOG(LogTemp, Log, TEXT("Login Success!"));
			StartMatching();
		};
		TFunction<void(FAccountRegisterResponse response)> RegisterResponseCallback = [this,LoginResponseCallback](FAccountRegisterResponse response)
		{
			UUserSaveGame* SaveGameInstance = Cast<UUserSaveGame>(UGameplayStatics::CreateSaveGameObject(UUserSaveGame::StaticClass()));
			SaveGameInstance->TransferCode = response.TransferCode;
			SaveGameInstance->UserID = response.UserID;
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("UserSaveSlot"), 0);
			URequestLogin* Request = NewObject<URequestLogin>(this);
			Request->UserID = response.UserID;
			Request->TransferCode = response.TransferCode;
			Request->RequestLogin(LoginResponseCallback);
		};
		URequestAccountRegister* Request = NewObject<URequestAccountRegister>(this);
		Request->RequestAccountRegister(RegisterResponseCallback);
	}
	else //既存のアカウントでログイン
	{
		if (UGameplayStatics::DoesSaveGameExist(TEXT("UserSaveSlot"), 0))
		{
			USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(TEXT("UserSaveSlot"), 0);
			UUserSaveGame* LoadedSave = Cast<UUserSaveGame>(LoadedGame);
			if (LoadedSave)
			{
				FString TransferCode = LoadedSave->TransferCode;
				int64 UserID = LoadedSave->UserID;
				TFunction<void(FLoginResponse response)> LoginResponseCallback = [this](FLoginResponse response)
				{
					USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(TEXT("UserSaveSlot"), 0);
					UUserSaveGame* SaveGameInstance = Cast<UUserSaveGame>(LoadedGame);
					SaveGameInstance->Token = response.Token;
					UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("UserSaveSlot"), 0);
					UE_LOG(LogTemp, Log, TEXT("Login Success!"));
					StartMatching();
				};
				URequestLogin* Request = NewObject<URequestLogin>(this);
				Request->UserID = UserID;
				Request->TransferCode = TransferCode;
				Request->RequestLogin(LoginResponseCallback);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to cast loaded save game to UUserSaveGame."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Save slot 'UserSaveSlot' does not exist."));
		}
	}
}

void ATitleGameModeBase::StartMatching()
{
	TFunction<void(FMatchResponse response)> MatchResponseCallback = [this](FMatchResponse response)
	{
		UE_LOG(LogTemp, Log, TEXT("Match API Success!"));
	};
	URequestMatch* Request = NewObject<URequestMatch>(this);
	Request->RequestMatch(MatchResponseCallback);
	
}
