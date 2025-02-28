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
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	float GetPoseAlphaThumbUp() const { return PoseAlphaThumbUp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	float GetPoseAlphaPoint() const { return PoseAlphaPoint; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	float GetPoseAlphaGrasp() const { return PoseAlphaGrasp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	float GetPoseAlphaIndexCurl() const { return PoseAlphaIndexCurl; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	float GetCurrentPoseAlphaThumbUp() const { return CurrentPoseAlphaThumbUp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	float GetCurrentPoseAlphaPoint() const { return CurrentPoseAlphaPoint; }
};
