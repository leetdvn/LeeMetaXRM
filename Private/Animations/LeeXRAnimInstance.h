// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LeeXRAnimInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ULeeXRAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	ULeeXRAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeVR Settings")
	bool bMirror;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings")
	float PoseAlphaGrasp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings")
	float PoseAlphaIndexCurl;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings")
	float PoseAlphaPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings")
	float PoseAlphaThumbUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeVR Settings")
	float CurrentPoseAlphaThumbUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeVR Settings")
	float CurrentPoseAlphaPoint;

};
