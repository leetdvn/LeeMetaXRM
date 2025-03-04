// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MotionControllerComponent.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRGrabbableActor.generated.h"


UENUM(BlueprintType)
enum class EGrabType : uint8
{
	EGT_Free UMETA(DisplayName = "Free"),
	EGT_Snap UMETA(DisplayName = "Snap"),
	EGT_None UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ELeeXRGrabableType : uint8
{
	LeeXROneHand UMETA(DisplayName = "One Hand"),
	LeeXRTwoHand UMETA(DisplayName = "Two Hand")
};

UCLASS(Abstract)
class LEEMETAXRM_API ALeeXRGrabbableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeeXRGrabbableActor();


	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	ELeeXRGrabableType GrabableType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	TObjectPtr<class UStaticMeshComponent> ActorMesh;

	UFUNCTION(BlueprintCallable)
	bool IsSimulation() { return ActorMesh->IsSimulatingPhysics(); }

	UFUNCTION(BlueprintCallable)
	void SetSimulation(bool bSimulate) { ActorMesh->SetSimulatePhysics(bSimulate); }

	void SetFreeze(bool bFreeze);

	UFUNCTION(BlueprintCallable)
	bool IsFreeze() { return FrezzeOnSnap; }

	//Get the rotation of the two controllers
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	FRotator GetTwoHandRotation(const  UMotionControllerComponent* FirstController, const  UMotionControllerComponent* SecondController);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeVR Settings|Properties")
	bool FrezzeOnSnap = false;

};
