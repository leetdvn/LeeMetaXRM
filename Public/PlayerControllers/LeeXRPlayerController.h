// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Definitions.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LeeXRPlayerController.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogLeeXRPlayerController, Log, All)

DECLARE_MEMORY_STAT_EXTERN(TEXT("ICTUPlayerController"), STAT_ICTUPlayerController, STATGROUP_ICTUMV, );
DECLARE_DWORD_COUNTER_STAT_EXTERN(TEXT("ICTUPlayerControllerMemories"), ICTUPlayerControllerMemories, STATGROUP_ICTUMV, );
/**
 * Class for the player controller
 *
  
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ALeeXRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALeeXRPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
