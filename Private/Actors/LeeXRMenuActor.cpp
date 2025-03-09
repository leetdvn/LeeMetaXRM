// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRMenuActor.h"
#include <APawn/LeeXRCharacter.h>
#include <Actors/LeeXRHandBase.h>
#include <Components/WidgetInteractionComponent.h>

// Sets default values
ALeeXRMenuActor::ALeeXRMenuActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALeeXRMenuActor::BeginPlay()
{
	Super::BeginPlay();
	
	WGInteractionRefLeft = FindWidgetInteractionReference(EControllerHand::Left);
	WGInteractionRefRight = FindWidgetInteractionReference(EControllerHand::Right);

	if (!IsValid(WGInteractionRefLeft) || !IsValid(WGInteractionRefRight))
	{
		LEE_LOG(LogLeeXRMenuActor, Warning, "Widget Interaction Reference Not Set");
	}

}

// Called every frame
void ALeeXRMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UWidgetInteractionComponent* ALeeXRMenuActor::FindWidgetInteractionReference(EControllerHand inType)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (APawn* Pawn = World->GetFirstPlayerController()->GetPawn())
		{
			if (ALeeXRCharacter* Character = Cast<ALeeXRCharacter>(Pawn))
			{
				return inType == EControllerHand::Left ?
					Character->GetHand(true)->GetWidgetInteraction():
					Character->GetHand(false)->GetWidgetInteraction();
			}
			LEE_LOG(LogLeeXRMenuActor,Warning,"Widget Interaction Reference Set");
			
		}
	}
	return nullptr;
}