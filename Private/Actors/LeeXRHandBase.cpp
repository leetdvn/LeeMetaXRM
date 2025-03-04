// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandBase.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Components/ArrowComponent.h>
#include <Definitions.h>
#include <EnhancedInputComponent.h>
#include <NiagaraDataInterfaceArrayFunctionLibrary.h>
#include <NavigationSystem.h>
#include "Actors/LeeXRTeleportActor.h"
#include "APawn/LeeXRCharacter.h"


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

	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
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

	XRCharacter = LeeXRGetCustomCharacter<ALeeXRCharacter>(this);

	if (!XRCharacter)
	{
		LeeScreenLog("XRCharacter is Null", FColor::Red);
		return;
	}
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


#pragma region Teleport

void ALeeXRHandBase::TeleportTrace(FVector StartPos, FVector ForwardVec)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	//Trace Teleport
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	float teleportSpped = 650.0f;
	float TeleportRadius = 3.6f;
	float LocalNavMeshCellHeight = 8.0f;
	FHitResult OutHit{};
	FVector LastTraceDestination = FVector::ZeroVector;

	UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(
		GetWorld(),
		OutHit,
		TeleportTracePathPositions,
		LastTraceDestination,
		StartPos,
		teleportSpped * ForwardVec,
		true,
		TeleportRadius,
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		0.0f,
		15.0f,
		2.0f,
		0.0f
	);


	//Update TeleportVisualizer Location
	TeleportTracePathPositions.Insert(StartPos, 0);
	FVector ProjectedLocation{};
	bool isTeleportValid = IsValidTeleportLocation(OutHit, ProjectedLocation);

	ProjectedTeleportLocation = FVector(ProjectedLocation.X, ProjectedLocation.Y, ProjectedLocation.Z - LocalNavMeshCellHeight);

	if (bValidTeleportLocation != isTeleportValid)
	{
		bValidTeleportLocation = isTeleportValid;
		//ActorToSpawn.GetDefaultObject()->GetRootComponent()->SetVisibility(bValidTeleportLocation);
	}

	TeleportRef->GetRootComponent()->SetVisibility(bValidTeleportLocation,true);
	//then Update Sequence 2


	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NiagaraComponent,
		TEXT("User.PointArray"),
		TeleportTracePathPositions);
	FHitResult* Hit = new FHitResult();

	FVector TeleportLocation = GetTeleportLocation(XRCharacter);

	TeleportRef->SetActorLocation(ProjectedTeleportLocation);
}

bool ALeeXRHandBase::IsValidTeleportLocation(FHitResult Hit, FVector& ProjectedLocation)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!NavSys) return false;


	return  NavSys->K2_ProjectPointToNavigation(
		GetWorld(),
		Hit.Location,
		ProjectedLocation,
		NULL,
		NULL,
		TeleportProjectPointToNavigationQueryExtent);
}

void ALeeXRHandBase::StartTeleportTrace()
{
	//Start Teleport Trace
	bTeleportTraceActive = true;
	TeleportTracePathPositions.Empty();
	//NiagaraComponent->SetVisibility(true);
	if (TeleportRef == nullptr)
		TeleportRef = LeeXRSPawnActorBP<AActor>(this, TeleportVisualizer);
}
void ALeeXRHandBase::TryTeleport()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	if (XRCharacter) {
		FVector TeleportLocation = GetTeleportLocation(XRCharacter);
		XRCharacter->TeleportTo(TeleportLocation, FRotator(0.0f, XRCharacter->GetActorRotation().Yaw, 0.0f),false,true);
	}

}
FVector ALeeXRHandBase::GetTeleportLocation(const ALeeXRCharacter* inXRCharacter)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return FVector::ZeroVector;

	if (inXRCharacter) {
		FVector CamVec = inXRCharacter->GetCameraLocation();
		FVector Minus = FVector(CamVec.X, CamVec.Y, inXRCharacter->GetActorRotation().Yaw);
		FVector TeleportLocation = ProjectedTeleportLocation - Minus;

		if (TeleportLocation.Z < 0)
			TeleportLocation.Z = 5;
		LeeScreenLog("Teleport Location %s", FColor::Green, *TeleportLocation.ToString());
		return TeleportLocation;
		//XRCharacter->TeleportTo(TeleportLocation, FRotator(0.0f, XRCharacter->GetActorRotation().Yaw, 0.0f), false, true);
	}

	return FVector::ZeroVector;
}
#pragma endregion Teleport