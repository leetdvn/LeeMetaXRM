// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ICTUGrabbableActor.generated.h"


UENUM(BlueprintType)
enum class EGrabType : uint8
{
	EGT_Free UMETA(DisplayName = "Free"),
	EGT_Snap UMETA(DisplayName = "Snap"),
	EGT_None UMETA(DisplayName = "None")
};

UCLASS(BlueprintType)
class LEEMETAXRM_API AICTUGrabbableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AICTUGrabbableActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings")
	TObjectPtr<class UStaticMeshComponent> ActorMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;


};
