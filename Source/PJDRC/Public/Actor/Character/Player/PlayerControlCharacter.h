// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/OriginalCharacter.h"
#include "GameFramework/Character.h"
#include "Data/Character/Player/PlayerData.h"
#include "PlayerControlCharacter.generated.h"
UCLASS()
class PJDRC_API APlayerControlCharacter : public AOriginalCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> MeshBody;
	
	// Sets default values for this character's properties
	APlayerControlCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Init(int32 InPlayerID);
	virtual void Init(UPlayerData InPlayerData);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
