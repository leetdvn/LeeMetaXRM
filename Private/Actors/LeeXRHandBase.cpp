// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandBase.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Definitions.h>
#include <EnhancedInputComponent.h>
#include <NiagaraDataInterfaceArrayFunctionLibrary.h>
#include <NavigationSystem.h>
#include "Actors/LeeXRTeleportActor.h"
#include "APawn/LeeXRCharacter.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


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

	PhysicContraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicContraint"));

	PhysicContraint->SetupAttachment(MotionController);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(MotionController);
}

bool ALeeXRHandBase::IsValidControllerType(ELeeXRHandType inType)
{
	return inType == ControllerType;
}

// Called when the game starts or when spawned
void ALeeXRHandBase::BeginPlay()
{
	Super::BeginPlay();
	
	///PROFILE
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	///Memory Stat
	ADDMEMORYSTAT(this, STAT_HandController);

	///Mapping Context input
	if (MenuContext) {
		InitializationContext(GetWorld(),DefaultContext ,0);
		InitializationContext(GetWorld(), MenuContext, 0);
		InitializationContext(GetWorld(), HandContext, 1);

	}
	///Set Up input Component
	SetInputComponent();

	///Initialize the setup
	InittializeSetup();

}

void ALeeXRHandBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
	///Reset Profile Memories
	SET_MEMORY_STAT(STAT_HandController, 0);


}

// Called when compiled
void ALeeXRHandBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Set Up Input Action Funtion
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
		//LeeScreenLog("Setting Input Component",FColor::Blue);
		LEE_LOG(LogLeeXRHandBase, Log, "Setting Input Component");
	}
}

// Initialize the setup
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

	if (TeleportRef == nullptr) {
		//Define Teleport Hand Action Left or Right
		if (TeleportValid()) {
			TeleportRef = LeeXRSPawnActorBP<AActor>(this, TeleportVisualizer);

			if (TeleportRef)
				TeleportRef->GetRootComponent()->SetVisibility(false, true);;
		}
	}



	//Set Show Debug
	//HandDebug->SetActive(bShowDebug);
	//HandDebug->SetVisibility(bShowDebug);
	//HandDebug->SetHiddenInGame(!bShowDebug);
}

// Finger Animation type Hand Controller Only
void ALeeXRHandBase::OnFingerAnimation(const FInputActionInstance& ActionInstance)
{
	
	SetFingerAnimationPose(HandSkeletal, ActionInstance);
	//SetFingerAnimationPose(HandDebug, ActionInstance);
}

//Has Overlap Actor Func
AActor* ALeeXRHandBase::HasOverlapActor(const USphereComponent* inSphere)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
	TArray<AActor*> OverlappingActors;
	inSphere->GetOverlappingActors(OverlappingActors);
	
	return OverlappingActors.IsEmpty() ? nullptr : OverlappingActors[0];
}

// Set Finger Animation Pose
void ALeeXRHandBase::SetFingerAnimationPose(USkeletalMeshComponent* inComponet, const FInputActionInstance ActionInstance)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	if (inComponet == nullptr) {
		LeeScreenLog("Hand Skeletal is Null", FColor::Red);
		return;
	};

	float ActValue = ActionInstance.GetValue().Get<float>();

	ETriggerEvent&& inEventType = ActionInstance.GetTriggerEvent();

	float PoseValueStartCancel = inEventType == ETriggerEvent::Completed ? ActValue : 0.0f;

	float PoseValueCancelCompleted = inEventType == ETriggerEvent::Triggered ? ActValue : 0.0f;

	FString EventName = UEnum::GetValueAsString(inEventType);

	FString ActName = ActionInstance.GetSourceAction()->GetName();

	if (ULeeXRAnimInstance* AnimIns = Cast<ULeeXRAnimInstance>(inComponet->GetAnimInstance())) {
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
				OnGrabObject() : OnReleaseObject();
		}
	}
}

// Switch Hand Type
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

	if (PROPERTYCHANGED(ALeeXRHandBase, ControllerType)) {
		if (ControllerType == ELeeXRHandType::LeeXRHandTracking)
		{
			//Do Something With Hand Tracking Editor Mode
			
		}
		else {
			//Do Something With Hand Controller Editor Mode
		}
	}
	else if (PROPERTYCHANGED(ALeeXRHandBase, HandType))
	{
		OnHandTypeChanged();
	}
}
#endif

UPrimitiveComponent* ALeeXRHandBase::GetPrimitiveComponent(bool isController) const
{
	return FindComponentByClass<UPrimitiveComponent>();
}

/// Grab Object
void ALeeXRHandBase::OnGrabOneHand()
{

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	//Hand Controller Grabs Objects
	//==============================================

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
				OnHandGrabledEvent.Broadcast();
				//Hand Controller Grabs Objects
				//CurrentGrabObject->OnGrab(HandSkeletal, GrabLocation);

				CurrentGrabObject->OnGrabObjects(MotionController);

			}
		}
	}

}

/// Grab Object 2
void ALeeXRHandBase::OnGrabObject()
{

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
	TArray<AActor*> OverlappingActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{};
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(this);

	float radius = GrabSphere != nullptr ? GrabSphere->GetScaledSphereRadius() : 10.f;
	bool isOverlaped = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		LeeXRGetWorldLocation(MotionController),
		radius,
		ObjectTypes,
		nullptr, 
		ActorToIgnore, 
		OverlappingActors);


	for (auto Actor : OverlappingActors)
	{
		//Tag Check
		if (Actor->ActorHasTag("Grabbable"))
		{
			CurrentGrabObject = TScriptInterface<ILeeXRInteraction>(Actor);
			if (CurrentGrabObject) {
				bIsHeld = true;
				OnHandGrabledEvent.Broadcast();
				CurrentGrabObject->OnGrabObjects(MotionController);
				//LeeScreenLog("Grabbing 2 %s", FColor::Green, *Actor->GetName());
				break;
			}

		}
	}
	FString msg = __FUNCTION__;
	LEE_LOG(LogLeeXRHandBase, Log, "Func %s", *msg);
	//LeeScreenLog("GTracking 2 %s", FColor::Green, *GetName());
}

/// Release Object 2
void ALeeXRHandBase::OnReleaseObject()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUController);
	if (CurrentGrabObject == nullptr ||
		HandSkeletal == nullptr) {
		return;
	}
	
	OnHandReleaseEvent.Broadcast();
	CurrentGrabObject->OnReleaseObjects(MotionController);
	CurrentGrabObject = nullptr;
	bIsHeld = false;

}

/// Grab Release
void ALeeXRHandBase::OnGrabOneHandRelease()
{

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	if (ControllerType == ELeeXRHandType::LeeXRHandTracking) return;

	if (CurrentGrabObject == nullptr ||
		HandSkeletal == nullptr) {
		
		return;
	}

	OnHandReleaseEvent.Broadcast();

	//Controller Release
	CurrentGrabObject->OnReleaseObjects(MotionController);
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

// Called when the hand type is changed
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

#pragma region Input Actions
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
	OnGrabObject();
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
		OnReleaseObject();
	}
}

// Called when the player is interacting
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
	WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
	//LeeScreenLog("Interact :%s", FColor::Green, *GetName());
}

// Called when the player is triggering
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

#pragma endregion Input Actions

#pragma region Teleport

void ALeeXRHandBase::TeleportTrace(FVector StartPos, FVector ForwardVec)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	if(TeleportRef)
		TeleportRef->SetActorHiddenInGame(false);
	//Trace Teleport
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

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
		TeleportDistance * ForwardVec,
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


	if (TeleportRef) {
		TeleportRef->GetRootComponent()->SetVisibility(bValidTeleportLocation, true);
		if(OutHit.bBlockingHit)
			TeleportRef->SetActorLocation(ProjectedTeleportLocation);
	}

	//then Update Sequence 2


	//UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NiagaraComponent,
	//	TEXT("User.PointArray"),
	//	TeleportTracePathPositions);
	//FHitResult* Hit = new FHitResult();

	FVector TeleportLocation = GetTeleportLocation(XRCharacter);
}

bool ALeeXRHandBase::IsValidTeleportLocation(FHitResult Hit, FVector& ProjectedLocation)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!NavSys) return false;

	ANavigationData* UseNavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);

	return  NavSys->K2_ProjectPointToNavigation(
		GetWorld(),
		Hit.Location,
		ProjectedLocation,
		UseNavData,
		NULL,
		TeleportProjectPointToNavigationQueryExtent);	
}

void ALeeXRHandBase::StartTeleportTrace()
{
	//Start Teleport Trace
	bTeleportTraceActive = true;
	TeleportTracePathPositions.Empty();
	//NiagaraComponent->SetVisibility(true);
	if (TeleportRef == nullptr) {
		TeleportRef = LeeXRSPawnActorBP<AActor>(this, TeleportVisualizer);
	}
}

void ALeeXRHandBase::TryTeleport()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	if (XRCharacter) {
		FVector TeleportLocation = GetTeleportLocation(XRCharacter);
		if (TeleportLocation.IsNearlyZero(0.0001f)) {
			LeeScreenLog("Teleport Location is Zero", FColor::Red);
		}
		if (TeleportLocation.Z <= 0) TeleportLocation.Z = 10;
		XRCharacter->TeleportTo(TeleportLocation, FRotator(0.0f, XRCharacter->GetActorRotation().Yaw, 0.0f), false, true);
		if(TeleportRef)
			TeleportRef->SetActorHiddenInGame(true);
	}
}

bool ALeeXRHandBase::TeleportValid()
{
	if (XRCharacter == nullptr) return false;

	ELeeXRTeleportHandAction TelportAct = XRCharacter->GetTeleportHandAction();

	bool TpAction = TelportAct == ELeeXRTeleportHandAction::LeeXRRight ? true : false;

	EControllerHand Direction = TpAction ? EControllerHand::Right : EControllerHand::Left;

	return HandType == Direction;
}

FVector ALeeXRHandBase::GetTeleportLocation(const ALeeXRCharacter* inXRCharacter)
{
	//Get Teleport Location from XRCharacter Camera
	UWorld* World = GetWorld();
	if (World == nullptr) return FVector::ZeroVector;

	if (inXRCharacter) {
		FVector CamVec = inXRCharacter->GetCameraLocation();
		FVector Minus = FVector(CamVec.X, CamVec.Y, inXRCharacter->GetActorRotation().Yaw);
		return ProjectedTeleportLocation - Minus;
	}

	return FVector::ZeroVector;
}
#pragma endregion Teleport