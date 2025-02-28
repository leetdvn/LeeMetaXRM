// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandTracking.h"
#include "OculusXRHandComponent.h"

ALeeXRHandTracking::ALeeXRHandTracking(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HandTrackingComp = CreateDefaultSubobject<UOculusXRHandComponent>(TEXT("HandTrackingComp"));
	HandTrackingComp->SetupAttachment(MotionController);

	WidgetInteraction->SetupAttachment(HandTrackingComp);

	ControllerType = ELeeXRHandType::LeeXRHandTracking;
}

void ALeeXRHandTracking::BeginPlay()
{
	Super::BeginPlay();

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	INC_MEMORY_STAT_BY(STAT_HandController, this->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));

}

void ALeeXRHandTracking::InitializeHandTracking()
{
}
