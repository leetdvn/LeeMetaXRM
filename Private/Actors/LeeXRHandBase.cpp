// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandBase.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Components/ArrowComponent.h>
#include <Definitions.h>
#include <EnhancedInputComponent.h>


DEFINE_STAT(STAT_ICTUController);
DEFINE_STAT(STAT_HandController)
// Sets default values
ALeeXRHandBase::ALeeXRHandBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteraction->SetupAttachment(MotionController);

}

bool ALeeXRHandBase::IsValidControllerType(ELeeXRHandType inType)
{
	
	return inType == ControllerType;
}

// Called when the game starts or when spawned
void ALeeXRHandBase::BeginPlay()
{
	Super::BeginPlay();
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	///Mapping Context
	if (MenuContext) {
		InitializationContext(GetWorld(), MenuContext, 0);
		InitializationContext(GetWorld(), HandContext, 1);
	}

	INC_MEMORY_STAT_BY(STAT_HandController, this->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));

	SetInputComponent();

	InittializeSetup();

}

void ALeeXRHandBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
	///Reset Profile Memories
	SET_MEMORY_STAT(STAT_ICTUController, 0);


}

// Called when compiled
void ALeeXRHandBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

void ALeeXRHandBase::SetInputComponent()
{
	UInputComponent* PlayerInputComponent = GetWorld()->GetFirstPlayerController()->InputComponent;
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		EnhancedInputComponent->BindAction(IA_MenuInteract, ETriggerEvent::Started, this, &ALeeXRHandBase::OnHandInteract);
		EnhancedInputComponent->BindAction(IA_MenuInteract, ETriggerEvent::Canceled, this, &ALeeXRHandBase::OnHandInteract);
		EnhancedInputComponent->BindAction(IA_MenuInteract, ETriggerEvent::Completed, this, &ALeeXRHandBase::OnHandInteract);

		EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Triggered, this, &ALeeXRHandBase::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Completed, this, &ALeeXRHandBase::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Canceled, this, &ALeeXRHandBase::OnFingerAnimation);
		//EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Triggered, this, &ALeeXRHandBase::OnHandTrigger);

		EnhancedInputComponent->BindAction(IA_HandThumpUp, ETriggerEvent::Started, this, &ALeeXRHandBase::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_HandThumpUp, ETriggerEvent::Canceled, this, &ALeeXRHandBase::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_HandThumpUp, ETriggerEvent::Completed, this, &ALeeXRHandBase::OnFingerAnimation);

		EnhancedInputComponent->BindAction(IA_CurlIndex, ETriggerEvent::Triggered, this, &ALeeXRHandBase::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_CurlIndex, ETriggerEvent::Canceled, this, &ALeeXRHandBase::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_CurlIndex, ETriggerEvent::Completed, this, &ALeeXRHandBase::OnFingerAnimation);

		EnhancedInputComponent->BindAction(IA_FingerPoint, ETriggerEvent::Started, this, &ALeeXRHandBase::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_FingerPoint, ETriggerEvent::Canceled, this, &ALeeXRHandBase::OnFingerAnimation);
		EnhancedInputComponent->BindAction(IA_FingerPoint, ETriggerEvent::Completed, this, &ALeeXRHandBase::OnFingerAnimation);

		EnhancedInputComponent->BindAction(IA_HandLog, ETriggerEvent::Started, this, &ALeeXRHandBase::LogReconize);
		LeeScreenLog("Setting Input Component",FColor::Blue);
	}
}

void ALeeXRHandBase::InittializeSetup()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
	///Load the hand assets
	if(GrabSphere)
		GrabSphere->SetSphereRadius(8.0f);

#if WITH_EDITOR
	if (WidgetInteraction) {
		WidgetInteraction->bShowDebug = true;
		WidgetInteraction->DebugColor = FColor::Green;
		WidgetInteraction->TraceChannel = ECollisionChannel::ECC_WorldDynamic;
	}
#endif
}

void ALeeXRHandBase::OnFingerAnimation(const FInputActionInstance& ActionInstance)
{
	SetFingerAnimationPose(HandSkeletal, ActionInstance);
}

AActor* ALeeXRHandBase::HasOverlapActor(const USphereComponent* inSphere)
{
	TArray<AActor*> OverlappingActors;
	inSphere->GetOverlappingActors(OverlappingActors);
	
	return OverlappingActors.IsEmpty() ? nullptr : OverlappingActors[0];
}

void ALeeXRHandBase::SetFingerAnimationPose(USkeletalMeshComponent* inComponet, const FInputActionInstance ActionInstance)
{
	if (inComponet == nullptr) {
		LeeScreenLog("Hand Skeletal is Null", FColor::Red);
		return;
	};
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	float ActValue = ActionInstance.GetValue().Get<float>();

	ETriggerEvent&& inEventType = ActionInstance.GetTriggerEvent();

	float PoseValueStartCancel = inEventType == ETriggerEvent::Completed ? ActValue : 0.0f;

	float PoseValueCancelCompleted = inEventType == ETriggerEvent::Triggered ? ActValue : 0.0f;

	FString EventName = UEnum::GetValueAsString(inEventType);

	FString ActName = ActionInstance.GetSourceAction()->GetName();

	ULeeXRAnimInstance* AnimIns = CastChecked<ULeeXRAnimInstance>(inComponet->GetAnimInstance());
	if (!AnimIns) return;

	if (ActName == this->IA_FingerPoint->GetName())
	{
		AnimIns->PoseAlphaPoint = PoseValueStartCancel;
	}
	else if (ActName == this->IA_CurlIndex->GetName())
	{
		AnimIns->PoseAlphaIndexCurl = PoseValueCancelCompleted;
	}
	else if (ActName == this->IA_HandThumpUp->GetName())
	{
		AnimIns->PoseAlphaThumbUp = PoseValueStartCancel;
	}
	else if (ActName == this->IA_Grasp->GetName())
	{
		AnimIns->PoseAlphaGrasp = PoseValueCancelCompleted;

		PoseValueCancelCompleted == 1 ?
			GraspObject() : GraspRelease();
	}

}

void ALeeXRHandBase::SetHandSwitch(bool isLeft)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	FString HandName = isLeft ? "Left" : "Right";

	MotionController->MotionSource = FName(*HandName);

	FVector LeftLoc = FVector(-8.5f, -3.5f, 4.5f);
	FVector RightLoc = FVector(-8.5f, 3.5f, 0.0f);
	//===================================
	FRotator LeftRot = FRotator(-80.0f, -180.0f, 78.f);
	FRotator RightRot = FRotator(80.0f, 0.0f, 78.f);

	if (HandSkeletal)
	{
		HandSkeletal->SetRelativeLocation(isLeft ? LeftLoc : RightLoc);
		HandSkeletal->SetRelativeRotation(isLeft ? LeftRot : RightRot);
	}

	if (ControllerType == ELeeXRHandType::LeeXRController) {
		if (ULeeXRAnimInstance* AnimIns = Cast<ULeeXRAnimInstance>(HandSkeletal->GetAnimInstance()))
		{
			AnimIns->bMirror = isLeft;
		}
	}

}

#if WITH_EDITOR
void ALeeXRHandBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property)
	{
		FName PropertyName = PropertyChangedEvent.Property->GetFName();
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ALeeXRHandBase,ControllerType))
		{
			if (ControllerType == ELeeXRHandType::LeeXRHandTracking)
			{
				//Do Something With Hand Tracking Editor Mode
	
			}
			else {
				//Do Something With Hand Controller Editor Mode
			}
		}
		else if (PropertyName == GET_MEMBER_NAME_CHECKED(ALeeXRHandBase, HandType))
		{
			OnHandTypeChanged();
		}
	}
}
#endif

/// Grab Object
void ALeeXRHandBase::GraspObject()
{

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	if (HandSkeletal == nullptr || 
		ControllerType == ELeeXRHandType::LeeXRHandTracking) return;

	AActor* OverlappingActor = HasOverlapActor(GrabSphere);
	if (OverlappingActor)
	{
		if (OverlappingActor)
		{
			CurrentGrabObject = TScriptInterface<ILeeXRInteraction>(OverlappingActor);
			if (CurrentGrabObject) {
				bIsHeld = true;
				FVector GrabLocation = HandSkeletal->GetComponentLocation();
				CurrentGrabObject->OnGrab(HandSkeletal, GrabLocation);
			}
		}
	}

}

/// Grab Release
void ALeeXRHandBase::GraspRelease()
{

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	if (ControllerType == ELeeXRHandType::LeeXRHandTracking) return;

	if (CurrentGrabObject == nullptr ||
		HandSkeletal == nullptr) {
		
		return;
	}

	CurrentGrabObject->OnRelease(HandSkeletal);
	CurrentGrabObject = nullptr;
	bIsHeld = false;
}

// Called every frame
void ALeeXRHandBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Find Actor to Grab
AActor* ALeeXRHandBase::FindActorToGrab(TArray<AActor*> &inActors, FString inTag)
{

	for (auto Actor : inActors)
	{
		if (Actor->ActorHasTag(*inTag))
		{
			LeeScreenLog("Grabbing Object %s", FColor::Green, *Actor->GetName());
			return Actor;
		}
	}
	return nullptr;
}

void ALeeXRHandBase::OnHandTypeChanged()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	switch (HandType)
	{
	case EControllerHand::Left: {
		MotionController->MotionSource = TEXT("Left");
		break;
	}
	case EControllerHand::Right:
		MotionController->MotionSource = TEXT("Right");
		break;
	}
}

// Called when the player is grabbing
void ALeeXRHandBase::OnHandGrabing(const FInputActionInstance& ActionInstance)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	FString ActName = ActionInstance.GetSourceAction()->GetName();
	//bool isLeft = ActName.EndsWith("Left");
	//TObjectPtr<ALeeXRHandBase> Hand = isLeft ?
	//	XRHandLeft :
	//	XRHandRight;

	//if (Hand && Hand->IsValidControllerType(ELeeXRHandType::LeeXRController))
	//	return Hand->GraspObject();
	GraspObject();
}

// Called when the player is releasing
void ALeeXRHandBase::OnHandRelease(const FInputActionInstance& ActionInstance)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	//FString ActName = ActionInstance.GetSourceAction()->GetName();
	//bool isLeft = ActName.EndsWith("Left");
	//TObjectPtr<ALeeXRHandBase> Hand = isLeft ?
	//	XRHandLeft :
	//	XRHandRight;

	if (IsValidControllerType(ELeeXRHandType::LeeXRController)) {
		GraspRelease();
	}
}

void ALeeXRHandBase::OnHandInteract(const FInputActionInstance& ActionInstance)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);


	//FString ActName = ActionInstance.GetSourceAction()->GetName();
	//bool isLeft = ActName.EndsWith("Left");
	//TObjectPtr<ALeeXRHandBase> Hand = isLeft ?
	//	XRHandLeft :
	//	XRHandRight;

	//ETriggerEvent TriggerEvent = ActionInstance.GetTriggerEvent();
	//if (Hand) {
	//	//LEE_LOG(LogLeeXRCharacter, Log, "Interact :%s",Hand->GetName());

	//	LeeScreenLog("Interact :%s", FColor::Green, *Hand->GetName());
	//	
	//}
	WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
}

void ALeeXRHandBase::OnHandTrigger(const FInputActionInstance& ActionInstance)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	//FString ActName = ActionInstance.GetSourceAction()->GetName();
	//bool isLeft = ActName.EndsWith("Left");
	//TObjectPtr<ALeeXRHandBase> Hand = isLeft ?
	//	XRHandLeft :
	//	XRHandRight;

	//if (Hand) {
	//	///Play Animation
	//	ALeeXRHandController* HandControl = LeeXRGetBaseClass<ALeeXRHandController>(Hand);
	//	if (HandControl)
	//	{
	//		FInputActionValue ActValue = ActionInstance.GetValue();
	//		HandControl->PlayAnimAction(EFingerInputType::XRGrasp, ActValue.Get<float>(), true);

	//	}
}
