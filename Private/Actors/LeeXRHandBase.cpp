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


		LeeScreenLog("Setting Input Component",FColor::Blue);
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

	TArray<AActor*> OverlappingActors;
	GrabSphere->GetOverlappingActors(OverlappingActors);

	if (!OverlappingActors.IsEmpty())
	{
		AActor* OverlappingActor = OverlappingActors[0];
		if (OverlappingActor)
		{
			LeeScreenLog("Grasp Object :%s", FColor::Green,*OverlappingActor->GetName());

			CurrentGrabObject = TScriptInterface<ILeeXRInteraction>(OverlappingActor);
			if (CurrentGrabObject) {
				bIsHeld = true;
				FVector GrabLocation = HandSkeletal->GetComponentLocation();
				CurrentGrabObject->OnGrab(HandSkeletal, GrabLocation);
				LeeScreenLog("Grabbing Object %s", FColor::Green, *OverlappingActor->GetName());

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
