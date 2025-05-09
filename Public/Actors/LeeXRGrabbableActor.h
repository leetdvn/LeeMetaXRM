// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LeeXRUltils.h>
#include <Animation/AnimLayerInterface.h>
#include <MotionControllerComponent.h>
#include "Interfaces/LeeXRInteraction.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameplayTagContainer.h>
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

	/// <summary>
	/// Check Tag Grabable and Tag Snapable
	/// </summary>
	/// <param name="inObjectTag"></param>
	/// <param name="isObject"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	bool IsTag(const FGameplayTag inObjectTag, bool isObject = true) const;

	UFUNCTION()
	bool IsTagString(const FString inObjectTag, bool isObject = true) const
	{
		return IsTag(FGameplayTag::RequestGameplayTag(FName(*inObjectTag)), isObject);
	}
	/// <summary>
	/// Get Tag
	/// </summary>
	/// <param name="isObject"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	FGameplayTag Tag(bool isObject=true) const { return isObject ? TagObject : TagElemental; }

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	ELeeXRGrabableType GrabableType;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties",meta =(DisplayName="Mass Default"))
	float LeeMassDefault;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties", meta = (DisplayName = "PhysicThreshold",ClampMin=50,ClampMax=100,DefaultsValue=75))
	float PhysicsGrabThreshold=75.f;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	bool bIsPhysicSimulate;


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

	void SetSmoothGrabableRelease(bool isGrabable = true, float inAngleDamping = 0.0, float inLinearDamping = .1f);


	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	UStaticMeshComponent* FindStaticMeshComponent(FString inName) const;

	FGameplayTag GetTagElemental() const { return TagElemental; }

	FGameplayTag GetTagObject() const { return TagObject; }
#pragma endregion Functions
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	EGrabType GrabType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	FGameplayTag TagElemental;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	FGameplayTag TagObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	bool bIsheld;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Base Grab Object
	virtual void OnGrabObjects(UMotionControllerComponent* inComponent) override;

	//Base Release Object
	virtual void OnReleaseObjects(UMotionControllerComponent* inComponent) override;

	/// <summary>
	///Settitng Init When Game Start
	/// </summary>
	virtual void InitSettings();

	/// <summary>
	/// Physics Contraint Implementation
	/// </summary>
	/// <param name="inMCComponent"></param>
	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void PhysicsContraintImplementation(UMotionControllerComponent *inMCComponent);

	/// <summary>
	/// Detach the object when the hand threshold exceed
	/// </summary>
	/// <param name="inMCComponent"></param>
	/// <param name="inSkeletal"></param>
	void DetachWhenHandThresholdExceed(UMotionControllerComponent* inMCComponent,USkeletalMeshComponent* inSkeletal);

	/// <summary>
	/// Detach the object when the hand threshold exceed
	/// </summary>
	/// <param name="inMCComponent"></param>
	/// <param name="inStatiMesh"></param>
	void DetachWhenHandThresholdExceed(UMotionControllerComponent* inMCComponent,UStaticMeshComponent* inStatiMesh);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	bool FrezzeOnSnap = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
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

	FTimerHandle TimerWeighted, TimerGrabable;

	///Grab One Hand
	
	
};