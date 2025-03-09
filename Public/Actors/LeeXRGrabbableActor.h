// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LeeXRUltils.h>
#include <Animation/AnimLayerInterface.h>
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
	EGT_Weighted UMETA(DisplayName = "Weighted"),
	EGT_None UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ELeeXRGrabableType : uint8
{
	LeeXROneHand UMETA(DisplayName = "One Hand"),
	LeeXRTwoHand UMETA(DisplayName = "Two Hand")
};

USTRUCT(BlueprintType)
struct FLeeXRGrabbableSockets
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	FString MainSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	FString SecondarySocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	FString SubSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	FString OptionalSocketName;

	FLeeXRGrabbableSockets() : MainSocketName(TEXT("")), SubSocketName(TEXT("")), OptionalSocketName(TEXT("")) {}
};


DECLARE_MEMORY_STAT_EXTERN(TEXT("LeeXRGrabable"), STAT_LeeXRGrabable, STATGROUP_ICTUMV, );
DECLARE_MEMORY_STAT_POOL_EXTERN(TEXT("LeeXRGrabableMemory"), STAT_LeeXRGrabableMemory, STATGROUP_ICTUMV, FPlatformMemory::MCR_PhysicalLLM,);


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

#pragma region Properties
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	ELeeXRGrabableType GrabableType;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties",meta =(DisplayName="Mass Default"))
	float LeeMassDefault;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties", meta = (DisplayName = "PhysicThreshold",ClampMin=50,ClampMax=100,DefaultsValue=75))
	float PhysicsGrabThreshold=75.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Required")
	TSubclassOf<UAnimInstance> AnimLayerClimb;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	FRotator SecondaryOffset;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	FLeeXRGrabbableSockets Sockets;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	TObjectPtr<UHapticFeedbackEffect_Base> HapticEffect;

#pragma endregion Properties

#pragma region Functions
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

	void SetHandSkeletalMesh(USkeletalMeshComponent* inSkeletal) { HandSkeletalMeshRef = inSkeletal; };
#pragma endregion Functions
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	EGrabType GrabType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	bool bIsheld;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnGrab(UObject* inComponent, const FVector& InGrabLocation) override;

	virtual void OnRelease(UObject* inComponent) override;

	//Base Grab Object
	virtual void OnGrabObjects(UMotionControllerComponent* inComponent) override;

	//Base Release Object
	virtual void OnReleaseObjects(UMotionControllerComponent* inComponent) override;

	/// <summary>
	///Settitng Init When Game Start
	/// </summary>
	virtual void InitSettings();

	void PhysicsContraintImplementation(UMotionControllerComponent *inMCComponent);

	void DetachWhenHandThresholdExceed(UMotionControllerComponent* inMCComponent,USkeletalMeshComponent* inSkeletal);

	void DetachWhenHandThresholdExceed(UMotionControllerComponent* inMCComponent,UStaticMeshComponent* inStatiMesh);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeVR Settings|Properties")
	bool FrezzeOnSnap = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeVR Settings|Properties")
	bool SnapHandMesh = false;


	UMotionControllerComponent*MainControllerRef;

	UMotionControllerComponent* SecondaryControllerRef;

	class UPhysicsConstraintComponent* GrabledContraintRef;

	USkeletalMeshComponent* HandSkeletalMeshRef = nullptr;

	FTransform CacheHandTransform;

	USceneComponent* MeshComponent;


private:


	void FreeGrababled(UMotionControllerComponent* inMotionController,bool isWeighted=false);

	
	TArray<class UPhysicsConstraintComponent*> GrabledConstraintsRefs;

	FTimerHandle TimerWeighted;

	///Grab One Hand
	
	
};