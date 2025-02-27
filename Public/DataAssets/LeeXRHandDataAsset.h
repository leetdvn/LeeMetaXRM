// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/LeeXRHandController.h"
#include "Actors/LeeXRHandTracking.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LeeXRHandDataAsset.generated.h"



USTRUCT(BlueprintType)
struct LEEMETAXRM_API FLeeHandData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALeeXRHandController> Controller;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALeeXRHandTracking> Tracking;


	FLeeHandData() :
		Controller(nullptr), Tracking(nullptr)
	{
	}
};



/**
 * 
 */
UCLASS(BlueprintType,Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ULeeXRHandDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "LeeVR Settings|Data")
	FString AssetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeVR Settings|Data")
	FLeeHandData Assets;
};
