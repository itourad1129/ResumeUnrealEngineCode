// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/OriginalCharacter.h"
#include "TestCharacter.generated.h"

UCLASS()
class PJDRC_API ATestCharacter : public AOriginalCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> EquipHairSkComp;

public:
	// Sets default values for this character's properties
	ATestCharacter();
	void OnMeshLoaded(TFunction<void(USkeletalMesh*)>);
	void LoadSpecificSwordMeshAsync(TFunction<void(USkeletalMesh*)> OnMeshLoaded);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	bool isPatchGameSuccess = false;
	bool isHairLoaded = false;
};
