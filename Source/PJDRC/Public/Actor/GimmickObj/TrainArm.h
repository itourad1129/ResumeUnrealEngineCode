// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "TrainArm.generated.h"

UCLASS()
class PJDRC_API ATrainArm : public AActor
{
	GENERATED_BODY()
	const FVector Box_Scale = FVector(0.5f,7.25f,0.5f);
	const FVector Head_Sphere_Location = FVector(260,0,0);
	const FVector Head_Sphere_Scale = FVector(18,18,18);
	const FVector Root_Sphere_Location = FVector(2210,0,0);
	const FVector Root_Sphere_Scale = FVector(18,18,18);
	const FVector Capsule_Location = FVector(1230.0,0,0);
	const FRotator Capsule_Rotate = FRotator(90,0,0);
	const FVector Capsule_Scale = FVector(35,45,54.75f);
	
public:	
	// Sets default values for this actor's properties
	ATrainArm();
	UPROPERTY(Transient, EditDefaultsOnly)
	TObjectPtr<USkeletalMesh> SkArmMesh;
	UPROPERTY(Transient, EditDefaultsOnly)
	TObjectPtr<UAnimSequenceBase> AnimationAsset;
	UPROPERTY(Transient, EditDefaultsOnly)
	TObjectPtr<UClass> AnimClass;
	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<USphereComponent> HeadSphereComponent;
	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<USphereComponent> RootSphereComponent;
	UPROPERTY(Transient, VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
