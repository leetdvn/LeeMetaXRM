// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/LeeXRHandBase.h"
#include "LeeXRHandController.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LogLeeXRHandController, Log, All);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ALeeXRHandController : public ALeeXRHandBase
{
	GENERATED_BODY()


public:
	/// <summary>
	/// Initialize the hand controller
	/// </summary>
	/// <param name="ObjectInitializer"></param>
	ALeeXRHandController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/// <summary>
	/// Get Skeeletal Mesh Component
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	USkeletalMeshComponent* GetHandSkeletal() const { return HandSkeletal.Get(); }

	/// <summary>
	/// Get Animation Blueprint Instance
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UAnimInstance* GetABPInstance() const { return HandSkeletal->GetAnimInstance(); }

	/// <summary>
	///	Grap Object
	/// </summary>
	virtual void GraspObject() override;

	/// <summary>
	/// Grasp Release
	/// </summary>
	virtual void GraspRelease() override;

	/// <summary>
	/// Initialize the hand controller
	/// </summary>
	virtual void InittializeSetup() override;



protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetInputComponent();


	UFUNCTION()
	void OnInputActionMove(const FInputActionInstance& ActionInstance);
private:


	/// <summary>
	/// Finger Animation Pose
	/// </summary>
	/// <param name="inComponet"></param>
	/// <param name="ActionInstance"></param>
	void SetFingerAnimationPose(USkeletalMeshComponent* inComponet, const FInputActionInstance ActionInstance);

};


