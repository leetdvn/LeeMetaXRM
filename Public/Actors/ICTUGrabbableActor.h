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

	UFUNCTION(BlueprintCallable)
	bool IsSimulation() { return ActorMesh->IsSimulatingPhysics(); }

	UFUNCTION(BlueprintCallable)
	void SetSimulation(bool bSimulate) { ActorMesh->SetSimulatePhysics(bSimulate); }

	void SetFreeze(bool bFreeze);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

	bool isFreeze = false;

};
