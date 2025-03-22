// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameInstance/LeeXRGameInstance.h>
#include "Definitions.h"
#include "DataAssets/LeeXRHandDataAsset.h"
#include "Actors/LeeXRHandBase.h"
#include "CoreMinimal.h"
#include "interfaces/LeeXRInterfaceHMD.h"
#include "GameFramework/Character.h"
#include "LeeXRCharacter.generated.h"



DEFINE_LOG_CATEGORY_STATIC(LogLeeXRCharacter,Log,All)

DECLARE_MEMORY_STAT_EXTERN(TEXT("ICTUCharacter"), STAT_ICTUCharacter, STATGROUP_ICTUMV, );
DECLARE_MEMORY_STAT_POOL_EXTERN(TEXT("ICTUCharacterMemory"), STAT_ICTUCharacterMemory, STATGROUP_ICTUMV,FPlatformMemory::MCR_PhysicalLLM, );


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeeXROnPoseMesh,AActor*,GrabableActor);

UENUM(BlueprintType)
enum class ELeeXRTeleportHandAction : uint8
{
	LeeXRLeft UMETA(DisplayName = "Left Action"),
	LeeXRRight UMETA(DisplayName = "Right Action")
};


/**
 * XR Character Class
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ALeeXRCharacter : public ACharacter, public ILeeXRInterfaceHMD
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ALeeXRCharacter();

	/// <summary>
	/// Get the hand animation instance
	/// </summary>
	/// <param name="isLeft"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UAnimInstance* GetHandAnimInstance(bool isLeft);

	/// <summary>
	/// Get Camera Location
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	FVector GetCameraLocation() const { return Camera->GetRelativeLocation(); }

	/// <summary>
	/// Get Teleport Hand Action
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	ELeeXRTeleportHandAction GetTeleportHandAction() const { return TeleportHandAction; }

	/// <summary>
	/// Set Teleport Hand Action
	/// </summary>
	/// <param name="inAction"></param>
	UFUNCTION(BlueprintCallable,  Category = "LeeXR|Func")
	void SetTeleportHandAction(ELeeXRTeleportHandAction inAction) { TeleportHandAction = inAction; }

	/// <summary>
	/// Get Hand Type 
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	ELeeXRHandType GetHandType() const { return HandType; }

	/// <summary>
	/// Get Hand Refercene
	/// </summary>
	/// <param name="isLeft"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	ALeeXRHandBase* GetHand(bool isLeft) const { return isLeft ? XRHandLeft : XRHandRight; }

	/// <summary>
	/// Get Widget Interaction
	/// </summary>
	/// <param name="isLeft"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UWidgetInteractionComponent* GetWidgetInteraction(bool isLeft) const { return isLeft ? XRHandLeft->GetWidgetInteraction() : XRHandRight->GetWidgetInteraction(); }

	/// <summary>
	///  Get the hand animation instance
	/// </summary>
	/// <param name="isLeft"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UMotionControllerComponent* GetMotionController(bool isLeft) const;


	/// <summary>
	/// Set Hand Type
	/// </summary>
	/// <param name="inType"></param>
	UFUNCTION(BlueprintCallable,  Category = "LeeXR|Func")
	void SetHandType(ELeeXRHandType inType) { HandType = inType; }

	/// <summary>
	/// Calculate Motion Controller Velocities
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void CalculateMotionControllerVelocities();

	/// <summary>
	/// Update Climbing
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void UpdateClimbing();

	/// <summary>
	/// Get Hand Physics
	/// </summary>
	/// <param name="isLeft"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	USkeletalMeshComponent* GetHandPhysics(bool isLeft) const { return isLeft ? HandPhysicsLeft : HandPhysicsRight; }

	UPROPERTY(BlueprintAssignable)
	FLeeXROnPoseMesh OnPoseMesh;

	void InitPhysicsContraints();

	void InitPhysicsAnimation();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	UAnimInstance* GetPhysicsAnimInstance(bool isLeft);

	void SetPhysicsAllBodyBlendWeight(float inWeight,bool isLeft);

	void PauseHandPhysics(bool isEnable = true,bool isLeft = true);

	void HandPhysicBlendToPoseable(class UPoseableMeshComponent* inPoseable,bool isLeft=true);

	enum class EICTUActionType GetCurrentActionType() const; 

	void OnHMDLevelChanged_Implementation(const FString& NewLevelName);

	void SetLockHMD(bool isLock) { Camera->bLockToHmd = isLock; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	int32 NextLevel = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnHMDOrientReset() override;



#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma region Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LeeXR Settings", meta = (DisplayName = "Teleport Action"))
	ELeeXRTeleportHandAction TeleportHandAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<UStaticMeshComponent> DisplayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	ELeeXRHandType HandType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<class USceneComponent> LeeXROrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<ALeeXRHandBase> XRHandLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<ALeeXRHandBase> XRHandRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<USkeletalMeshComponent> HandPhysicsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<USkeletalMeshComponent> HandPhysicsRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<UPhysicalAnimationComponent> AnimPhysicsRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<UPhysicalAnimationComponent> AnimPhysicsLeft;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "LeeXR Settings|Data")
	ULeeXRHandDataAsset* DataLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Data")
	ULeeXRHandDataAsset* DataRight;


#pragma endregion Components

#pragma region Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputMappingContext> HandMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_Turn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_LMenuToogle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_RMenuToogle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_Grasp;


#pragma endregion Input

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// Test Moving IA Move
	/// </summary>
	UFUNCTION()
	void OnMoving();

	UFUNCTION()
	void OnHandGrabing(const FInputActionInstance& ActionInstance);

	UFUNCTION()
	void OnResetOrientation();

	UFUNCTION()
	void OnTurn(const FInputActionInstance& ActionInstance);
private:

	TObjectPtr<class ALeeXRGrabbableActor> HeldLeftObject;

	TObjectPtr<class ALeeXRGrabbableActor> HeldRightObject;

	bool isClimbing;

	FVector RightHandVelocity;
	FVector LeftHandVelocity;
	FVector LastFrameRightHandLocation;
	FVector LastFrameLeftHandLocation;
	/// <summary>
	/// Hand Initialize
	/// </summary>
	ALeeXRHandBase* HandInitialize(ELeeXRHandType inType,bool isLeft);

	template<typename T>
	ALeeXRHandBase* InitializeHandActor(const FString inLeftHandPath);

	template<typename T>
	ALeeXRHandBase* InitializeHandActor(TSubclassOf<T> inClass);

	TWeakObjectPtr<ALeeXRCharacter> Self = nullptr;


	//Init SpawnLoaction
	void InitSpawnLocation();

	//Init VR Tracking
	void InitVRTrackingOrigin();
};

template<typename T>
inline ALeeXRHandBase* ALeeXRCharacter::InitializeHandActor(const FString inHandPath)
{
	if (inHandPath.IsEmpty()) return nullptr;
	// Path Actor Blueprint
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	// Spawn the hands
	ALeeXRHandBase* HandBase = LeeXRSPawnActorBP<T>(this, inHandPath);
	if (HandBase == nullptr) return nullptr;
	HandBase->AttachToComponent(LeeXROrigin, AttachRules);

	return nullptr;
}

template<typename T>
inline ALeeXRHandBase* ALeeXRCharacter::InitializeHandActor(TSubclassOf<T> inClass)
{
	if (inClass == nullptr) return nullptr;
	// Path Actor Blueprint

	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	// Spawn the hands
	ALeeXRHandBase* HandBase = LeeXRSPawnActorBP<T>(this,inClass);
	if (HandBase == nullptr) return nullptr;
	bool isSuccess = HandBase->K2_AttachToComponent(LeeXROrigin,NAME_None,EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true);
	if (!isSuccess) {
		LeeScreenLog("Hand Attach Failed", FColor::Red);
	}

	return HandBase;
}
