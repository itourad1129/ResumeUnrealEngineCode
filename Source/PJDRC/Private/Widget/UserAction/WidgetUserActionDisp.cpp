// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/UserAction/WidgetUserActionDisp.h"

#include "Kismet/GameplayStatics.h"

void UWidgetUserActionDisp::NativeConstruct()
{
	SetAlignmentInViewport(FVector2d(0,0));
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	FTimerHandle handle;
	timerManager.SetTimer(handle, this, &UWidgetUserActionDisp::UpdateWidgetPosition, 0.2f, true);
	Super::NativeConstruct();
	// Here is where I typically bind delegates,
	// and set up default appearance
}

void UWidgetUserActionDisp::UpdateWidgetPosition()
{
	APlayerController* aPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if(aPlayerController)
	{
		// ワールド座標をスクリーン座標に変換
		FVector2D aScreenLocation;
		aPlayerController->ProjectWorldLocationToScreen(TargetPosition, aScreenLocation);
		SetPositionInViewport(aScreenLocation);
		SetVisibility(ESlateVisibility::Visible);
	}
}