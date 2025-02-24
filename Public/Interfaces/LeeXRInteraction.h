// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LeeXRInteraction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULeeXRInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEEMETAXRM_API ILeeXRInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnGrab(USkeletalMeshComponent* inComponent,FVector &InGrabLocation) = 0;
	virtual void OnRelease(USkeletalMeshComponent* inComponent) = 0;
};
