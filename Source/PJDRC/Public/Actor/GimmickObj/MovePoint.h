// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "MovePoint.generated.h"

UCLASS()
class PJDRC_API AMovePoint : public AActor
{
	GENERATED_BODY()
private:
	const FString USER_ACTION_WIDGET_PATH = "/Game/OriginalContent/Widget/WB_UserActionDisp.WB_UserActionDisp_C";
public:
	// Sets default values for this actor's properties
	UPROPERTY(Transient, EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> NiagaraSystemAsset;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleCollision;
	const FVector Capsule_Scale = FVector(6.75f,6.75f,6.75f);
	const FVector Capsule_Location = FVector(0,0,150);
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;
	const FVector Sphere_Scale = FVector(35.0f,35.0f,35.0f);
	const FVector Sphere_Location = FVector(0,0,0);
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> Piller;
	TObjectPtr<UUserWidget> UserWidget;
	AMovePoint();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
