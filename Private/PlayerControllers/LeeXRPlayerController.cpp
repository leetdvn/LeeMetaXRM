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
	ADDMEMORYSTAT(this, ICTUPlayerControllerMemories);


}

void ALeeXRPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SET_CYCLE_COUNTER(STAT_ICTUPlayerController, 0);
	SET_MEMORY_STAT(ICTUPlayerControllerMemories, 0);


}

void ALeeXRPlayerController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
