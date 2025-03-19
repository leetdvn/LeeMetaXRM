// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <PhysicsEngine/PhysicsAsset.h>
#include <NiagaraComponent.h>
#include <LeeXRUltils.h>
#include <Interfaces/LeeXRInteraction.h>
#include "MotionControllerComponent.h"
#include <Components/WidgetInteractionComponent.h>
#include "Animations/LeeXRAnimInstance.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRHandBase.generated.h"


UENUM(BlueprintType)
enum class ELeeXRHandType : uint8
{
	LeeXRController UMETA(DisplayName = "Controller"),
	LeeXRHandTracking UMETA(DisplayName = "HandTracking"),
	LeeXRHandPhysics UMETA(DisplayName = "HandPhysics")
};

UENUM(BlueprintType)
enum class EFingerInputType : uint8
{
	XRThumbUp UMETA(DisplayName = "ThumUp"),
	XRPoint UMETA(DisplayName = "Point"),
	XRIndex UMETA(DisplayName = "Index"),
	XRGrasp UMETA(DisplayName = "Grasp")
};


DEFINE_LOG_CATEGORY_STATIC(LogLeeXRHandBase, Log, All);

using namespace LeeXRUltils;
/**
 *
 */

DECLARE_MEMORY_STAT_EXTERN(TEXT("ICTUController"), STAT_ICTUController, STATGROUP_ICTUMV, );
DECLARE_MEMORY_STAT_POOL_EXTERN(TEXT("HandController"), STAT_HandController, STATGROUP_ICTUMV, FPlatformMemory::MCR_PhysicalLLM,);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLogRecognizer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeeXROnHandGrabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeeXROnHandRelease);



UCLASS(Abstract)
class LEEMETAXRM_API ALeeXRHandBase : public AActor
{
	GENERATED_BODY()
	


public:	
	// Sets default values for this actor's properties
	ALeeXRHandBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/// <summary>
	/// Get Hand Anim Instance
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UAnimInstance* GetHandAnimInstance() { return HandSkeletal->GetAnimInstance(); }

	/// <summary>
	/// Valid Controller Type
	/// </summary>
	/// <param name="inType"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	bool IsValidControllerType(ELeeXRHandType inType);

	/// <summary>
	/// Valid Grab Function
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	bool IsValidGrab() { return bIsHeld; }

	/// <summary>
	/// Get Motion Controller Location
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	FVector GetMotionControllerLocation() { return MotionController->GetComponentLocation(); }

	/// <summary>
	/// Get MC Location to World
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	FVector GetMCLocationToWorld() { return LeeXRGetWorldLocation(MotionController); }

	/// <summary>
	/// Get Motion Controller Forward Vector
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	FVector GetMotionControllerForwardVector() { return MotionController->GetForwardVector(); }

	/// <summary>
	/// Get Widget Interaction Component
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UWidgetInteractionComponent* GetWidgetInteraction() { return WidgetInteraction; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UMotionControllerComponent* GetMotionController() { return MotionController; }
	/// <summary>
	/// Get Skeletal Mesh Component
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	USkeletalMeshComponent* GetHandSkeletal() const { return HandSkeletal.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UPrimitiveComponent* GetPrimitiveComponent(bool isController = true) const;

	/// <summary>
	/// Get Physics Constraint Component
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UPhysicsConstraintComponent* GetPhysicsConstraint() const { return PhysicContraint.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	EControllerHand GetHandType() const { return HandType; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UPhysicsConstraintComponent* GetPhysicsContraint() const { return PhysicContraint.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UPhysicsConstraintComponent* GetGrabsContraint() const { return GrabsContraint.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UWidgetInteractionComponent* GetWidgetInteractionComponent() const { return WidgetInteraction.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	bool IsHandLeft() const { return HandType == EControllerHand::Left; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UPhysicsConstraintComponent* GetPhysicsContraints() const { return PhysicContraint.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UPoseableMeshComponent* GetPoseableMesh() const { return PoseableMesh.Get(); }

	void PoseableSpawned(USceneComponent* inParent,USkeletalMesh* inAsset,USkeletalMeshComponent* inSkeletalRef);

	void PoseableDestroyed();

	/// <summary>
	/// Grab Two Hand
	/// </summary>
	virtual void OnGrabObject();

	/// <summary>
	/// Grab Two Hand Release
	/// </summary>
	virtual void OnReleaseObject();

#pragma region WIDGETS
	void ToogleWidgetInteraction(bool isEnable);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Widgets")
	TSubclassOf<AActor> WGMenu;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Widgets")
	TObjectPtr<AActor> WGActionMenu;
		
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void SetInputComponent();

	virtual void InittializeSetup();

	virtual void OnFingerAnimation(const FInputActionInstance& ActionInstance);

	UFUNCTION()
	void LogReconize() { OnLogRecognizer.Broadcast(); }

	UPROPERTY(BlueprintAssignable, Category = "LeeXR|Delegate")
	FOnLogRecognizer OnLogRecognizer;

	AActor* HasOverlapActor(const USphereComponent* inSphere);

	void SetFingerAnimationPose(USkeletalMeshComponent* inComponet, const FInputActionInstance ActionInstance);

	void SetHandSwitch(bool isLeft);
	bool bIsCanGrasp;

	bool bIsShow = false;

	TObjectPtr<ALeeXRCharacter> XRCharacter=nullptr;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma region Teleport

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void TeleportTrace(FVector StartPos, FVector ForwardVec);

	bool IsValidTeleportLocation(FHitResult Hit, FVector& ProjectedLocation);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void StartTeleportTrace();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void TryTeleport();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	bool TeleportValid();
	/// <summary>
	/// Get Teleport Location from XRCharacter Camera
	/// </summary>
	/// <param name="inXRCharacter"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	FVector GetTeleportLocation(const class ALeeXRCharacter* inXRCharacter);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Varibles")
	TArray<FVector> TeleportTracePathPositions;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Varibles")
	FVector TeleportProjectPointToNavigationQueryExtent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Varibles")
	FVector ProjectedTeleportLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Varibles")
	bool bValidTeleportLocation = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Varibles")
	bool bTeleportTraceActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Varibles")
	bool bShowDebug=true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Varibles")
	TObjectPtr<AActor> TeleportRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Actors")
	TObjectPtr<class UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Actors")
	TSubclassOf<AActor> TeleportVisualizer;

#pragma endregion

#pragma region Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class UOculusXRHandComponent> OculusHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class UMotionControllerComponent> MotionController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class UWidgetInteractionComponent> WidgetInteraction;

	/**Collision Sphere**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class USphereComponent> GrabSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class UOculusXRHandComponent> HandTrackingComp;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<class USkeletalMeshComponent> HandSkeletal=nullptr;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<class UHandPoseRecognizer> HandPoseRecognizer = nullptr;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<class UPhysicsConstraintComponent> PhysicContraint = nullptr;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<class UPhysicsConstraintComponent> GrabsContraint = nullptr;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class UPoseableMeshComponent> PoseableMesh=nullptr;


#pragma endregion

#pragma region HandData
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeXR Settings")
	EControllerHand HandType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings")
	ELeeXRHandType ControllerType;

	UPROPERTY(BlueprintAssignable, Category = "LeeXR Settings|Delegates")
	FLeeXROnHandGrabled OnHandGrabledEvent;

	UPROPERTY(BlueprintAssignable, Category = "LeeXR Settings|Delegates")
	FLeeXROnHandRelease OnHandReleaseEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings")
	bool bMirrorAnimation = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings")
	bool bIsHeld = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings", meta = (ClampMin = "650", ClampMax = "2000",DefaultValue="650.0"))
	float TeleportDistance = 650;
	/// <summary>
	/// Interface
	/// </summary>
	TScriptInterface<ILeeXRInteraction> CurrentGrabObject;

#pragma endregion

#pragma region Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputMappingContext> MenuContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputMappingContext> HandContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputMappingContext> DefaultContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_Grasp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_MenuInteract;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_FingerPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_CurlIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_HandThumpUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_HandLog;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "LeeXR Settings|Input")
	TObjectPtr<class UInputAction> IA_MenuAction;

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void OnMenuAction(const FInputActionInstance& ActionInstance);

#pragma endregion

private:

	AActor* FindActorToGrab(TArray<AActor*> &inActors, FString inTag);

	void OnHandTypeChanged();

	UFUNCTION()
	void OnHandTrigger(const FInputActionInstance& ActionInstance);
	/// <summary>
	/// Input Action for Grabing
	/// </summary>
	/// <param name="ActionInstance"></param>
	UFUNCTION()
	void OnHandGrabing(const FInputActionInstance& ActionInstance);

	/// <summary>
	/// Input Action for Releasing
	/// </summary>
	/// <param name="ActionInstance"></param>
	UFUNCTION()
	void OnHandRelease(const FInputActionInstance& ActionInstance);

	UFUNCTION()
	void OnHandInteract(const FInputActionInstance& ActionInstance);

};
