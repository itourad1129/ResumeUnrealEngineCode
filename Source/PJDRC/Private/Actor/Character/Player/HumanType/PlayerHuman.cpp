// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/Player/HumanType/PlayerHuman.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "OriginalGameInstance.h"
#include "Data/Asset/Item/Equip/Human/HumanEquipPAL.h"
#include "Data/Item/Equip/Human/HumanEquipItemData.h"
#include "Http/RequestChunkVersion.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/OriginalAssetManager.h"

class UOriginalAssetManager;
// Sets default values
APlayerHuman::APlayerHuman()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	EquipBodySkComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipBodySkComp"));
	EquipBodySkComp->SetupAttachment(GetMesh());
	EquipFaceSkComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipFaceSkComp"));
	EquipFaceSkComp->SetupAttachment(GetMesh());
	EquipHairSkComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipHairSkComp"));
	EquipHairSkComp->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void APlayerHuman::BeginPlay()
{
	Super::BeginPlay();
#if !UE_SERVER //spannerエミュレーター環境構築まで一旦サーバー実行禁止
	GetMesh()->SetRenderCustomDepth(true);
	FollowCamera->SetRelativeLocation(FVector(-800.0f,0.0f,500.0f));
	FollowCamera->SetRelativeRotation(FRotator(-15.0f,0.0f,0.0f));
	EquipBodySkComp->SetAnimClass(GetMesh()->AnimClass);
	
	FHumanEquipDataTable humanEquipData;
	UHumanEquipItemDataTableDataAsset::GetHumanEquipData(this->GetWorld(),1,humanEquipData);
	UKismetSystemLibrary::PrintString(this,humanEquipData.Name, true, true, FColor::Cyan, 30.f, TEXT("None"));

	UGameInstance* gameInstance = GetGameInstance();
	UOriginalGameInstance* oGameIns = Cast<UOriginalGameInstance>(gameInstance);
	
	//装備アセットDL
	EquipAssetData = NewObject<UEquipItemAssetData>( this );
	EquipAssetData->SetEquipAssetData(oGameIns, EquipHairSkComp, EquipHairMesh, 1110);
	EquipAssetData->SetEquipAssetData(oGameIns, EquipBodySkComp, EquipBodyMesh, 1111);
	EquipAssetData->SetEquipAssetData(oGameIns, EquipFaceSkComp, EquipFaceMesh, 1112);

	oGameIns->ConnectServer(TEXT("127.0.0.1"));
#endif
}

// Called every frame
void APlayerHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerHuman::OnSkeletalMeshLoaded(FPrimaryAssetId LoadedId)
{
	IsMounted = true;
	GetLoadedId = LoadedId;
}

void APlayerHuman::LoadSkeletalMesh()
{
	if(IsMounted  && !LoadSk)
	{
		UOriginalAssetManager& AssetManager = UOriginalAssetManager::GetAssetManager();
		UHumanEquipPAL* DataAsset = Cast<UHumanEquipPAL>(AssetManager.GetPrimaryAssetObject(GetLoadedId));

		if (DataAsset)
		{
			//UHumanEquipPAL* SwordDataAsset = Cast<UHumanEquipPAL>(DataAsset);
			UHumanEquipPAL* SwordDataAsset = Cast<UHumanEquipPAL>(DataAsset);
			if(SwordDataAsset != nullptr)
			{
				//TSoftObjectPtr<UObject> SoftObjectPtr = SwordDataAsset->ExplicitAssets[0];
				USkeletalMesh* LoadSkeletalMesh = Cast<USkeletalMesh>(EquipHairMesh.Get());
				if (LoadSkeletalMesh == nullptr)
				{
					LoadSkeletalMesh = EquipHairMesh.LoadSynchronous();
				}
				EquipHairSkComp->SetSkeletalMesh(LoadSkeletalMesh);
				EquipHairSkComp->SetAnimClass(GetMesh()->AnimClass);
			}
			else
			{
				UKismetSystemLibrary::PrintString(this,"faild Cast<UPrimaryAssetLabel>", true, true, FColor::Cyan, 30.f, TEXT("None"));
			}
		}
		else
		{
			UKismetSystemLibrary::PrintString(this,"faild DataAsset", true, true, FColor::Cyan, 30.f, TEXT("None"));
		}
	}
}

void APlayerHuman::NoChunkCheck()
{
	FPrimaryAssetId Pid = FPrimaryAssetId("HumanEquipPAL:EquipHuman_Body_0");
	UOriginalAssetManager& AssetManager = UOriginalAssetManager::GetAssetManager();
		UHumanEquipPAL* DataAsset = Cast<UHumanEquipPAL>(AssetManager.GetPrimaryAssetObject(Pid));

		if (DataAsset)
		{
			//UHumanEquipPAL* SwordDataAsset = Cast<UHumanEquipPAL>(DataAsset);
			UHumanEquipPAL* SwordDataAsset = Cast<UHumanEquipPAL>(DataAsset);
			if(SwordDataAsset != nullptr)
			{
				//TSoftObjectPtr<UObject> SoftObjectPtr = SwordDataAsset->ExplicitAssets[0];
				USkeletalMesh* LoadSkeletalMesh = Cast<USkeletalMesh>(EquipHairMesh.Get());
				if (LoadSkeletalMesh == nullptr)
				{
					LoadSkeletalMesh = EquipHairMesh.LoadSynchronous();
				}
				EquipHairSkComp->SetSkeletalMesh(LoadSkeletalMesh);
				EquipHairSkComp->SetAnimClass(GetMesh()->AnimClass);
			}
			else
			{
				UKismetSystemLibrary::PrintString(this,"faild Cast<UPrimaryAssetLabel>", true, true, FColor::Cyan, 30.f, TEXT("None"));
			}
		}
		else
		{
			UKismetSystemLibrary::PrintString(this,"faild DataAsset", true, true, FColor::Cyan, 30.f, TEXT("None"));
		}
}

// Called to bind functionality to input
void APlayerHuman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerHuman::Move);
	}
}

void APlayerHuman::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerHuman::TestHttpRequest()
{
	URequestChunkVersion* HttpRequestInstance = NewObject<URequestChunkVersion>(this);
	TFuture<TOptional<FChunkVersionData>> Future = HttpRequestInstance->SendGetRequestAndWait();
	
	AsyncTask(ENamedThreads::AnyThread, [Future = MoveTemp(Future)]() mutable
	{
		TOptional<FChunkVersionData> Result = Future.Get();
		if (Result.IsSet())
		{
			AsyncTask(ENamedThreads::GameThread, [Result]() 
			{
				const FChunkVersionData& Data = Result.GetValue();
				UE_LOG(LogTemp, Log, TEXT("VersionID: %d, DeploymentName: %s"), Data.VersionID, *Data.DeploymentName);
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, []()
			{
				UE_LOG(LogTemp, Error, TEXT("HTTP Request Failed or JSON Parsing Error"));
			});
		}
	});
}

