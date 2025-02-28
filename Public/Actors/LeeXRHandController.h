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
	ALeeXRHandController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	USkeletalMeshComponent* GetHandSkeletal() const { return HandSkeletal.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	UAnimInstance* GetABPInstance() const { return HandSkeletal->GetAnimInstance(); }

	UFUNCTION(BlueprintCallable, Category = "LeeVR|Func")
	void SetHandSwitch(bool isLeft);

	virtual void GraspObject() override;

	virtual void GraspRelease() override;


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetInputComponent() override;
	UFUNCTION(BlueprintCallable)
	void OnFingerAnimation(const FInputActionInstance& ActionInstance);


private:

	void InittializeSetup();

	void SetFingerAnimationPose(USkeletalMeshComponent* inComponet, const FInputActionInstance ActionInstance);

};


