// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/NPC/ObjectType/NPCCubeObject.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ANPCCubeObject::ANPCCubeObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NiagaraComponents.SetNum(Niagara_Comp_Num);
	BoneSocketNames.SetNum(Niagara_Comp_Num);
	for(int i=0; i < Niagara_Comp_Num; i++)
	{
		FName name = "NiagaraComp_";
		FString base = name.ToString();
		base.Append(FString::FromInt(i));
		FName newName = FName(*base);
		NiagaraComponents[i] = CreateDefaultSubobject<UNiagaraComponent>(newName);
		NiagaraComponents[i]->SetupAttachment(GetMesh());
	}
}

// Called when the game starts or when spawned
void ANPCCubeObject::BeginPlay()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetMesh()->SetSkeletalMesh(SkCubeMesh);
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->PlayAnimation(AnimationAsset,true);
	for(int i=0; i < Niagara_Comp_Num; i++)
	{
		NiagaraComponents[i]->SetAsset(NiagaraSystemAsset);
		FName name = "Bone_";
		FString base = name.ToString();
		FString printfString = FString::Printf(TEXT("%03d"), i + 1);
		base.Append(printfString);
		base.Append(TEXT("_endSocket"));
		FName newName = FName(*base);
		BoneSocketNames[i] = newName;
	}
	TargetBoneSocketLocation = GetMesh()->GetSocketLocation(TargetBoneSocketName);
	for(int i=0; i < Niagara_Comp_Num; i++)
	{
		if(NiagaraComponents[i] != nullptr && NiagaraComponents[i]->GetAsset() != nullptr)
		{
			NiagaraComponents[i]->SetWorldLocation(GetMesh()->GetSocketLocation(BoneSocketNames[i]));
			NiagaraComponents[i]->SetVariableVec3("SetUserVelocity",FVector(500,0,0));
			FRotator fRotator = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(BoneSocketNames[i]),TargetBoneSocketLocation);
			NiagaraComponents[i]->SetVariableVec3("SetUserYPR",FVector(fRotator.Yaw,fRotator.Pitch,fRotator.Roll));
		}
	}
	GetCharacterMovement()->GravityScale = 0;
	Super::BeginPlay();
}

// Called every frame
void ANPCCubeObject::Tick(float DeltaTime)
{
	for(int i=0; i < Niagara_Comp_Num; i++)
	{
		if(NiagaraComponents[i] != nullptr && NiagaraComponents[i]->GetAsset() != nullptr)
		{
			NiagaraComponents[i]->SetWorldLocation(GetMesh()->GetSocketLocation(BoneSocketNames[i]));
			NiagaraComponents[i]->SetVariableVec3("SetUserVelocity",FVector(500,0,0));
			FRotator fRotator = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(BoneSocketNames[i]),TargetBoneSocketLocation);
			NiagaraComponents[i]->SetVariableVec3("SetUserYPR",FVector(fRotator.Yaw,fRotator.Pitch,fRotator.Roll));
		}
	}
	Super::Tick(DeltaTime);
}

