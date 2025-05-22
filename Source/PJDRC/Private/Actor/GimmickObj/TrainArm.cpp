// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GimmickObj/TrainArm.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATrainArm::ATrainArm(): AActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->OnComponentBeginOverlap.AddDynamic(this,&ATrainArm::OnOverlapBegin);
	SkeletalMeshComponent->OnComponentEndOverlap.AddDynamic(this,&ATrainArm::OnComponentOverlapEnd);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(SkeletalMeshComponent);
	BoxComponent->SetRelativeScale3D(Box_Scale);
	
	HeadSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HeadSphereComponent"));
	HeadSphereComponent->SetupAttachment(SkeletalMeshComponent);
	HeadSphereComponent->SetRelativeScale3D(Head_Sphere_Scale);
	HeadSphereComponent->SetRelativeLocation(Head_Sphere_Location);
	
	RootSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));
	RootSphereComponent->SetupAttachment(SkeletalMeshComponent);
	RootSphereComponent->SetRelativeScale3D(Root_Sphere_Scale);
	RootSphereComponent->SetRelativeLocation(Root_Sphere_Location);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(SkeletalMeshComponent);
	CapsuleComponent->SetRelativeScale3D(Capsule_Scale);
	CapsuleComponent->SetRelativeRotation(Capsule_Rotate);
	CapsuleComponent->SetRelativeLocation(Capsule_Location);
}

// Called when the game starts or when spawned
void ATrainArm::BeginPlay()
{
	SkeletalMeshComponent->SetAnimClass(AnimClass);
	SkeletalMeshComponent->SetSkeletalMesh(SkArmMesh);
	SkeletalMeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SkeletalMeshComponent->SetRenderCustomDepth(true);
	
	SkeletalMeshComponent->PlayAnimation(AnimationAsset,false);
	SkeletalMeshComponent->SetPosition(0,false);
	SkeletalMeshComponent->Stop();
	
	UAnimSingleNodeInstance* singleNode = Cast<class UAnimSingleNodeInstance>(SkeletalMeshComponent->AnimScriptInstance);
	if(singleNode != nullptr)
	{
		singleNode->SetReverse(true);
	}
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ATrainArm::OnOverlapBegin);
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this,&ATrainArm::OnOverlapBegin);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this,&ATrainArm::OnComponentOverlapEnd);
	Super::BeginPlay();
}

// Called every frame
void ATrainArm::Tick(float DeltaTime)
{
	if(BoxComponent != nullptr && SkeletalMeshComponent != nullptr)
	{
		FVector vec;
		FRotator rot;
		SkeletalMeshComponent->GetSocketWorldLocationAndRotation("Bone_002_socket",vec,rot);
		FTransform transform = UKismetMathLibrary::MakeTransform(vec,rot,Box_Scale);
		BoxComponent->SetWorldTransform(transform);
	}
	Super::Tick(DeltaTime);
}

void ATrainArm::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OverlappedComponent == BoxComponent)
	{
		if(Cast<UStaticMeshComponent>(OtherComp))
		{
			SkeletalMeshComponent->Stop();
		}
	}
	if(OverlappedComponent ==CapsuleComponent)
	{
		if(Cast<ACharacter>(OtherActor))
		{
			UAnimSingleNodeInstance* singleNode = Cast<class UAnimSingleNodeInstance>(SkeletalMeshComponent->AnimScriptInstance);
			if(singleNode != nullptr)
			{
				singleNode->SetReverse(false);
			}
			SkeletalMeshComponent->Play(false);
		}
	}
}

void ATrainArm::OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OverlappedComponent ==CapsuleComponent)
	{
		if(Cast<ACharacter>(OtherActor))
		{
			UAnimSingleNodeInstance* singleNode = Cast<class UAnimSingleNodeInstance>(SkeletalMeshComponent->AnimScriptInstance);
			if(singleNode != nullptr)
			{
				singleNode->SetReverse(true);
			}
			SkeletalMeshComponent->Play(false);
		}
	}
}

