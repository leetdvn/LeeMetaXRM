// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRMenuActor.h"
#include <APawn/LeeXRCharacter.h>
#include <Actors/LeeXRHandBase.h>
#include <Components/WidgetInteractionComponent.h>
#include <MotionControllerComponent.h>
#include <Kismet/GameplayStatics.h>
#include <EnhancedInputComponent.h>

// Sets default values
ALeeXRMenuActor::ALeeXRMenuActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OriginComponent = CreateDefaultSubobject<USceneComponent>(TEXT("OriginComponent"));
	SetRootComponent(OriginComponent);

	WGComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WGComponent"));
	SetRootComponent(OriginComponent);

}

// Called when the game starts or when spawned
void ALeeXRMenuActor::BeginPlay()
{
	Super::BeginPlay();
	
	//Set Input Component
	InitializationContext(GetWorld(), MenuActionContext, 0);

	SetReference();

}

void ALeeXRMenuActor::SetupActorInputComponent()
{
	UInputComponent* PlayerInputComponent = GetWorld()->GetFirstPlayerController()->InputComponent;
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (!IsValid(WGInteractionRefLeft) || !IsValid(WGInteractionRefRight))
		{
			EnhancedInputComponent->BindAction(IA_MenuAction_Left, ETriggerEvent::Started, this, &ALeeXRMenuActor::OnInputActiveMenu);
			EnhancedInputComponent->BindAction(IA_MenuAction_Left, ETriggerEvent::Completed, this, &ALeeXRMenuActor::OnInputActiveMenu);
			LEE_LOG(LogLeeXRMenuActor, Warning, "Widget Interaction Reference Not Set");
		}
	}
}

// Called every frame
void ALeeXRMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	///Move Update Comfortable Location
	OnMoveComfortableLocation();
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

void ALeeXRMenuActor::OnInputActiveMenu(const FInputActionInstance& ActionInstance)
{
	FString ActName = ActionInstance.GetSourceAction()->GetName();
	ETriggerEvent TriggerEvent = ActionInstance.GetTriggerEvent();
	LEE_LOG(LogLeeXRMenuActor, Warning, "Action Name %s", *ActName);

	UWidgetInteractionComponent* MControl = ActName.EndsWith("Left") ? WGInteractionRefLeft : WGInteractionRefRight;
	OnActionMenu(MControl, TriggerEvent);
}

void ALeeXRMenuActor::OnActionMenu(UWidgetInteractionComponent* inComponent, ETriggerEvent inEvent)
{
	if (!IsValid(inComponent)) return;

	switch (inEvent)
	{
		case ETriggerEvent::Triggered: { break; }
		case ETriggerEvent::Started: { return inComponent->PressPointerKey(EKeys::LeftMouseButton); }
		case ETriggerEvent::Ongoing: { break; }
		case ETriggerEvent::Canceled: { break; }
		case ETriggerEvent::Completed: { return inComponent->ReleasePointerKey(EKeys::LeftMouseButton); }

	}
}

UMotionControllerComponent* ALeeXRMenuActor::FindMotionControllerReference(bool isLeftHand)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (APawn* Pawn = World->GetFirstPlayerController()->GetPawn())
		{
			if (ALeeXRCharacter* Character = Cast<ALeeXRCharacter>(Pawn))
			{
				return Character->GetMotionController(isLeftHand);
			}
			LEE_LOG(LogLeeXRMenuActor, Warning, "Widget Interaction Reference Set");

		}
	}
	return nullptr;
}

void ALeeXRMenuActor::SetReference()
{
	WGInteractionRefLeft = FindWidgetInteractionReference(EControllerHand::Left);
	WGInteractionRefRight = FindWidgetInteractionReference(EControllerHand::Right);

	if (auto* XRCharacter = LeeXRGetCustomCharacter<ALeeXRCharacter>(this))
	{
		bool IsLeft = XRCharacter->GetTeleportHandAction() == ELeeXRTeleportHandAction::LeeXRLeft ? true : false;

		MotionControllerRef = FindMotionControllerReference(IsLeft);

		if (!IsValid(WGInteractionRefLeft) ||
			!IsValid(WGInteractionRefRight) ||
			!IsValid(MotionControllerRef))
		{
			LEE_LOG(LogLeeXRMenuActor, Error, "Widget Interaction Reference Not Set");
		}

	}

}

void ALeeXRMenuActor::OnMoveComfortableLocation()
{
	if (!IsValid(MotionControllerRef)) return;

	if (auto const WGComp = FindComponentByClass<UWidgetComponent>())
	{
		LookAtComponent<UWidgetComponent>(this,WGComp, true);
	}

}
