// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/LeeXRPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include <HeadMountedDisplayFunctionLibrary.h>
#include <Kismet/GameplayStatics.h>

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
	///Stat
	//ADDMEMORYSTAT(this, ICTUPlayerControllerMemories);


#if !UE_BUILD_SHIPPING
	//LeeScreenLog("Hand Type %s", FColor::Green, *HandType.ToString());
	UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("ShowFlag.Collision"));
	UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("Show collision"));
	UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("STAT ICTUMV"));

#endif
}

void ALeeXRPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Reset Profile Memories Counter
	SET_CYCLE_COUNTER(STAT_ICTUPlayerController, 0);
	SET_MEMORY_STAT(ICTUPlayerControllerMemories, 0);
	//SET_MEMORY_STAT(STAT_ICTUMV_TotalMemories, 0);

	FString Reason = UEnum::GetValueAsString(EndPlayReason);
	LEE_LOG(LogLeeXRPlayerController, Log, "End Play %s",*Reason);

}

#if WITH_EDITOR

void ALeeXRPlayerController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ALeeXRPlayerController::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif