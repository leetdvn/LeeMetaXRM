// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/LeeXRHandBase.h"
#include "LeeXRHandTracking.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LogLeeXRHandTracking, Log, All);

/**
 * 
 */
UCLASS(BlueprintType)
class LEEMETAXRM_API ALeeXRHandTracking : public ALeeXRHandBase
{
	GENERATED_BODY()
	
public:
	ALeeXRHandTracking(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
	virtual void GraspObject() override;

	virtual void GraspRelease() override;
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void InitializeHandTracking();
};
