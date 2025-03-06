// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LeeXRUltils.h>
#include <MotionControllerComponent.h>
#include "Interfaces/LeeXRInteraction.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeeXROnGrabObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeeXROnReleaseObject);

using namespace LeeXRUltils;
/**
 *
 */

UCLASS(Abstract)
class LEEMETAXRM_API ALeeXRGrabbableActor : public AActor ,public ILeeXRInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeeXRGrabbableActor();


	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	ELeeXRGrabableType GrabableType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Required")
	TSubclassOf<UAnimInstance> AnimLayerClimb;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	FRotator SecondaryOffset;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	FString SocketName;

	UPROPERTY(BlueprintAssignable, Category = "LeeXR Settings|Delegates")
	FLeeXROnGrabObject LeeXROnGrabObject;

	UPROPERTY(BlueprintAssignable, Category = "LeeXR Settings|Delegates")
	FLeeXROnReleaseObject LeeXROnReleaseObject;

	void SetFreeze(bool bFreeze);

	UFUNCTION(BlueprintCallable)
	bool IsFreeze() { return FrezzeOnSnap; }

	//Get the rotation of the two controllers
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	FRotator GetTwoHandRotation(const  UMotionControllerComponent* FirstController, const  UMotionControllerComponent* SecondController);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	bool TryFindHandMeshOnController(UMotionControllerComponent* inController, USkeletalMeshComponent*& outMesh);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void CaptureHandMesh(UMotionControllerComponent* inController,bool isLeft=true);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void ReleaseHandMesh(UMotionControllerComponent*& inController, bool isLeft = true);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnGrab(UObject* inComponent, const FVector& InGrabLocation) override;

	virtual void OnRelease(UObject* inComponent) override;

	virtual void OnGrabObjects(UMotionControllerComponent* inComponent) override;

	virtual void OnReleaseObjects(UMotionControllerComponent* inComponent) override;

	virtual void InitSettings();


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeVR Settings|Properties")
	bool FrezzeOnSnap = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeVR Settings|Properties")
	bool SnapHandMesh = false;


private:

	USkeletalMeshComponent* HandSkeletalMesh = nullptr;

	FTransform CacheHandTransform;

	
};
