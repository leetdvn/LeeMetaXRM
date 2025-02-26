// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	LeeXRHandTracking UMETA(DisplayName = "HandTracking")
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

//for (TActorIterator<ADayCycle> ActorItr(()); ActorItr; ++ActorItr)
//{
//	ActorItr->DoSonething();
//}
using namespace LeeXRUltils;
/**
 *
 */
UCLASS(Abstract)
class LEEMETAXRM_API ALeeXRHandBase : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	ALeeXRHandBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	UAnimInstance* GetHandAnimInstance() { return HandSkeletal->GetAnimInstance(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	bool IsValidGrab() { return bIsHeld; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	bool IsValidControllerType(ELeeXRHandType inType) { return ControllerType == inType; }

	virtual void GraspObject();

	virtual void GraspRelease();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma region Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class UOculusXRHandComponent> OculusHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class UMotionControllerComponent> MotionController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class UWidgetInteractionComponent> WidgetInteraction;

	/**Collision Sphere**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class USphereComponent> GrabSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class UOculusXRHandComponent> HandTrackingComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class USkeletalMeshComponent> HandSkeletal=nullptr;

#pragma endregion

#pragma region HandData
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LeeVR Settings")
	EControllerHand HandType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeVR Settings")
	ELeeXRHandType ControllerType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings")
	bool bMirrorAnimation = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings")
	bool bIsHeld = false;

	/// <summary>
	/// Interface
	/// </summary>
	TScriptInterface<ILeeXRInteraction> CurrentGrabObject;

#pragma endregion
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	AActor* FindActorToGrab(TArray<AActor*> &inActors, FString inTag);
};
