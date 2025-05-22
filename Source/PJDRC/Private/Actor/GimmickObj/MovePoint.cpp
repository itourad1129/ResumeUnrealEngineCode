// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GimmickObj/MovePoint.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/UserAction/WidgetUserActionDisp.h"


// Sets default values
AMovePoint::AMovePoint(): AActor()
{
	// SceneComponentを作成する
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	// SceneComponentをRootComponentに設定する
	DefaultSceneRoot->SetupAttachment(RootComponent);
	//RootComponent = DefaultSceneRoot;

	// ポータル侵入判定用
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("EnterCheckFlg"));
	CapsuleCollision->SetWorldScale3D(Capsule_Scale);
	CapsuleCollision->SetWorldLocation(Capsule_Location);
	CapsuleCollision->SetupAttachment(DefaultSceneRoot);
	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this,&AMovePoint::OnOverlapBegin);
	CapsuleCollision->OnComponentEndOverlap.AddDynamic(this,&AMovePoint::OnComponentOverlapEnd);
	
	//ポータルエフェクト発生距離判定
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ActiveFlg"));
	SphereCollision->SetWorldScale3D(Sphere_Scale);
	SphereCollision->SetWorldLocation(Sphere_Location);
	SphereCollision->SetupAttachment(DefaultSceneRoot);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this,&AMovePoint::OnOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this,&AMovePoint::OnComponentOverlapEnd);

	//ポータルエフェクト生成
	Piller = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PillerEffect"));
	Piller->SetAsset(NiagaraSystemAsset);
	Piller->SetupAttachment(DefaultSceneRoot);
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMovePoint::BeginPlay()
{
	Piller->SetAsset(NiagaraSystemAsset);
	Piller->SetActive(false);
	//SphereCollision->SetHiddenInGame(false);
	//CapsuleCollision->SetHiddenInGame(false);
	Super::BeginPlay();
}

// Called every frame
void AMovePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMovePoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UKismetSystemLibrary::PrintString(this,"Hit", true, true, FColor::Cyan, 30.f, TEXT("None"));
	if(Cast<ACharacter>(OtherActor))
	{
		if(OverlappedComponent == CapsuleCollision)
		{
			FString path = USER_ACTION_WIDGET_PATH;
			TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();
			// PlayerControllerを取得する
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if(WidgetClass && PlayerController)
			{
				// Widgetを作成する
				UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);
				// Viewportに追加する
				UserWidget->AddToViewport(0);
				UserWidget->SetVisibility(ESlateVisibility::Hidden);
				if(UWidgetUserActionDisp* WidgetUserAction = Cast<UWidgetUserActionDisp>(UserWidget))
				{
					WidgetUserAction->TargetPosition = GetActorLocation();
				}
			}
		}
		if(OverlappedComponent == SphereCollision)
		{
			Piller->SetActive(true);
		}
	}
}

void AMovePoint::OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Cast<ACharacter>(OtherActor))
	{
		if(OverlappedComponent == CapsuleCollision)
		{
			if(UserWidget)
			{
				UserWidget->RemoveFromParent();
			}
		}
		if(OverlappedComponent == SphereCollision)
		{
			Piller->SetActive(false);
		}
	}
}


