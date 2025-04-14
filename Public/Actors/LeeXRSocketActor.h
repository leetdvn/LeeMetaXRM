// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRSocketActor.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LeeXRSocketActor, Log, All);

class UWidgetComponent;

UCLASS()
class LEEMETAXRM_API ALeeXRSocketActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletonBasic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PhysicsCollisionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties", meta = (AllowPrivateAccess = "true", DefaultValue = "FColor::Green"))
	FColor PreviewColor = FColor::Green;

public:	
	// Sets default values for this actor's properties
	ALeeXRSocketActor();
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	TObjectPtr<UWidgetComponent> StatusAct;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	//template<typename T>
	//void LookAtComponent(T* TargetComponent, bool isYawOnly);

	virtual void InitSetting();

private:

	TObjectPtr<class ALeeXRCharacter> XRCharacter;
};
