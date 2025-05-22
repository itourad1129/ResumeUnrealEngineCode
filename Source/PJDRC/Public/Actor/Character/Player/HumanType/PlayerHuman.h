// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/OriginalCharacter.h"
#include "Data/Asset/Item/Equip/EquipItemAssetData.h"
#include "Logging/LogMacros.h"
#include "PlayerHuman.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config=Game)
class PJDRC_API APlayerHuman : public AOriginalCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	//UPROPERTY(Transient, EditDefaultsOnly)
	//TObjectPtr<USkeletalMesh> EquipBodyMesh;

	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> EquipBodySkComp;

	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> EquipFaceSkComp;

	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> EquipHairSkComp;

	UPROPERTY(Transient, EditDefaultsOnly)
	TSoftObjectPtr<USkeletalMesh> EquipHairMesh;

	UPROPERTY(Transient, EditDefaultsOnly)
	TSoftObjectPtr<USkeletalMesh> EquipFaceMesh;

	UPROPERTY(Transient, EditDefaultsOnly)
	TSoftObjectPtr<USkeletalMesh> EquipBodyMesh;
public:
	// Sets default values for this character's properties
	APlayerHuman();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnSkeletalMeshLoaded(FPrimaryAssetId LoadedId);
	void LoadSkeletalMesh();
	void NoChunkCheck();
	FPrimaryAssetId GetLoadedId;
	bool IsMounted = false;
	bool LoadSk = false;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY()
	TObjectPtr<UEquipItemAssetData> EquipAssetData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

private:
	bool isPatchGameSuccess = false;
	bool isHairLoaded = false;
	// HTTPリクエストテスト
	void TestHttpRequest();
};
