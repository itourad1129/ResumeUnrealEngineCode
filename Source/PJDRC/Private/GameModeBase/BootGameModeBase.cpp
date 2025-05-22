// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/BootGameModeBase.h"

#include "OriginalGameInstance.h"

ABootGameModeBase::ABootGameModeBase()
{
	
}

void ABootGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (UOriginalGameInstance* GI = Cast<UOriginalGameInstance>(GetGameInstance()))
	{
		GI->MasterDataManager->OnMasterDataDownloadCompleted.AddDynamic(this, &ABootGameModeBase::OnMasterDataReady);
	}
}

void ABootGameModeBase::OnMasterDataReady()
{
	UE_LOG(LogTemp, Log, TEXT("Next Level"));
	
	if (TargetLevelAsset.ToSoftObjectPath().IsValid())
	{
		TargetLevelAsset.LoadSynchronous();
		
		FStringAssetReference LevelRef = TargetLevelAsset.ToSoftObjectPath();
		FName LevelName = FName(*LevelRef.GetAssetName());
		UGameplayStatics::OpenLevel(this, LevelName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetLevelAsset is not set!"));
	}
}
