// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRMenuActor.h"
#include <APawn/LeeXRCharacter.h>
#include <Actors/LeeXRHandBase.h>
#include <Components/WidgetInteractionComponent.h>
#include <MotionControllerComponent.h>
#include <Kismet/GameplayStatics.h>
#include <EnhancedInputComponent.h>
#include <SceneComponent.h>
#include "Components/WidgetComponent.h"

// Sets default values
ALeeXRMenuActor::ALeeXRMenuActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OriginComponent = CreateDefaultSubobject<USceneComponent>(TEXT("OriginComponent"));
	SetRootComponent(OriginComponent);

	WGComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WGComponent"));
	SetRootComponent(OriginComponent);

	Cursor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cursor"));
	Cursor->SetupAttachment(WGComponent);
}

// Called when the game starts or when spawned
void ALeeXRMenuActor::BeginPlay()
{
	Super::BeginPlay();
	
	//Set Input Component
	InitializationContext(GetWorld(), MenuActionContext, 0);

	SetupActorInputComponent();

	SetReference();

}

void ALeeXRMenuActor::SetupActorInputComponent()
{
	UInputComponent* PlayerInputComponent = GetWorld()->GetFirstPlayerController()->InputComponent;
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (!IsValid(WGInteractionRefLeft) || !IsValid(WGInteractionRefRight))
		{
			EnhancedInputComponent->BindAction(IA_MenuAction_Left, ETriggerEvent::Started, this, &ALeeXRMenuActor::OnInputActiveMenu);
			EnhancedInputComponent->BindAction(IA_MenuAction_Right, ETriggerEvent::Completed, this, &ALeeXRMenuActor::OnInputActiveMenu);
			EnhancedInputComponent->BindAction(IA_MenuCursor_Left, ETriggerEvent::Triggered, this, &ALeeXRMenuActor::OnMenuCursorActiveMenu);
			EnhancedInputComponent->BindAction(IA_MenuCursor_Right, ETriggerEvent::Triggered, this, &ALeeXRMenuActor::OnMenuCursorActiveMenu);

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

void ALeeXRMenuActor::OnMenuCursorActiveMenu(const FInputActionInstance& ActionInstance)
{
	FString ActName = ActionInstance.GetSourceAction()->GetName();
	FVector2D ActValue = ActionInstance.GetValue().Get<FVector2D>();

	//Update Cursor Location
	UpdateCursorLocation(ActValue);

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
			if (IsValid(XRCharacter))
			{
				return XRCharacter->GetMotionController(isLeftHand);
			}

		}
	}
	return nullptr;
}

void ALeeXRMenuActor::SetReference()
{
	WGInteractionRefLeft = FindWidgetInteractionReference(EControllerHand::Left);
	WGInteractionRefRight = FindWidgetInteractionReference(EControllerHand::Right);
	XRCharacter = LeeXRGetCustomCharacter<ALeeXRCharacter>(this);
	LEE_CHECK(XRCharacter)

	if (IsValid(XRCharacter))
	{
		bool IsLeft = XRCharacter->GetTeleportHandAction() == ELeeXRTeleportHandAction::LeeXRLeft ? true : false;

		MotionControllerRef = FindMotionControllerReference(!IsLeft);
		ActiveMenuRight = !IsLeft;

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
		LookAtComponent<UWidgetComponent>(this,WGComp,false);
	}

	//Update Widget Location
	UpdateWidgetLocation();

}

void ALeeXRMenuActor::UpdateWidgetLocation()
{
	if (MotionControllerRef ==nullptr || WGComponent  == nullptr|| !IsValid(MotionControllerRef)) return;

	FVector MCAim = LeeXRGetWorldLocation(MotionControllerRef);

	FVector CameraLoc = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();

	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(MCAim, CameraLoc);

	FVector FWDistVec = UKismetMathLibrary::GetForwardVector(LookAt) * MenuDistanceToWardsCamera;

	FVector UpVec = UKismetMathLibrary::GetUpVector(LookAt) * MenuDistanceToWardsCamera;

	FVector NewLocation = MCAim + FWDistVec + UpVec;
	WGComponent->SetWorldLocation(FVector(NewLocation.X + 20,NewLocation.Y, NewLocation.Z));

	//LeeScreenLog("Location : %s", FColor::Green,*NewLocation.ToString());
}


// Update Cursor Location
void ALeeXRMenuActor::UpdateCursorLocation(FVector2D inputVec)
{
	if (Cursor == nullptr) return;

	EControllerHand HandType = ActiveMenuRight ? EControllerHand::Right : EControllerHand::Left;

	if (auto WidgetInteraction = FindWidgetInteractionReference(HandType)) {

		if (IsValid(WidgetInteraction))
		{
			if (WidgetInteraction->IsOverHitTestVisibleWidget()) return;

			float cursorSpd = CursorSpeed * inputVec.X;
			FVector CursorLoc = Cursor->GetRelativeLocation();

			float ClampValue = CursorLoc.Y + cursorSpd;

			float ClampedY = FMath::Clamp(ClampValue, -1.f, CursorLocationLimitY);

			float Speedabs = FMath::Abs(CursorSpeed) * inputVec.Y;

			float ClampedZ = FMath::Clamp(CursorLoc.Z + Speedabs, -1.f, CursorLocationLimitZ);

			Cursor->SetRelativeLocation(FVector(0, ClampedY, ClampedZ));
		}
	}
}

