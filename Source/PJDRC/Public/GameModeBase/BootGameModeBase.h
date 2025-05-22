// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/MasterDataManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "BootGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PJDRC_API ABootGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public :
	ABootGameModeBase();
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> TargetLevelAsset;
private:
	UFUNCTION()
	void OnMasterDataReady();
};
