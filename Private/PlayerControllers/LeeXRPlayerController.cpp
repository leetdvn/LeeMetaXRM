// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/LeeXRPlayerController.h"

DEFINE_STAT(STAT_ICTUPlayerController);
DEFINE_STAT(ICTUPlayerControllerMemories);

ALeeXRPlayerController::ALeeXRPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ALeeXRPlayerController::BeginPlay()
{
	Super::BeginPlay();


	///Cycle Counter
	LEE_SCOPE_CYCLE_COUNTER(ICTUPlayerController);

	///Memory Stat
	INC_MEMORY_STAT_BY(ICTUPlayerControllerMemories, this->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));


}

void ALeeXRPlayerController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
