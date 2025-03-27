// Fill out your copyright notice in the Description page of Project Settings.


#include "APawn/LeeXRCharacter.h"

#include <Camera/CameraComponent.h>
#include <EnhancedInputComponent.h>
#include <InputAction.h>
#include <EnhancedInputSubsystems.h>
#include <HeadMountedDisplayFunctionLibrary.h>
#include "Actors/LeeXRHandBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include <LeeXRUltils.h>
#include <Actors/LeeXRHandController.h>
#include <Actors/LeeXRHandTracking.h>
#include <Animations/LeeXRAnimInstance.h>
#include <NavigationSystem.h>
#include <NiagaraDataInterfaceArrayFunctionLibrary.h>
#include <NiagaraComponent.h>
#include <NiagaraFunctionLibrary.h>
#include "Actors/LeeXRGrabbableActor.h"
#include "Actors/LeeXRHandPhysics.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include <PoseableMeshComponent.h>
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include <GameInstance/LeeXRGameInstance.h>

using namespace LeeXRUltils;

DEFINE_STAT(STAT_ICTUCharacter);
DEFINE_STAT(STAT_ICTUCharacterMemory);
DEFINE_STAT(STAT_ICTUMV_TotalMemories);

#define LOCTEXT_NAMESPACE "LeeMetaXRModules"

float MemoriesSize;
// Sets default values
ALeeXRCharacter::ALeeXRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	LeeXROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("LeeXROrigin"));
	LeeXROrigin->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(LeeXROrigin);

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetupAttachment(Camera);

	HandPhysicsLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandPhysicsLeft"));
	HandPhysicsRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandPhysicsRight"));

	HandPhysicsLeft->SetupAttachment(LeeXROrigin);
	HandPhysicsRight->SetupAttachment(LeeXROrigin);

	AnimPhysicsLeft = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("AnimPhysicsLeft"));
	AnimPhysicsRight = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("AnimPhysicsRight"));

	if (Self == nullptr)
		Self = this;
}

// Get the hand animation instance
UAnimInstance* ALeeXRCharacter::GetHandAnimInstance(bool isLeft)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	ALeeXRHandBase* Hand = isLeft ?
		XRHandLeft :
		XRHandRight;

	ALeeXRHandController* HandController = Cast<ALeeXRHandController>(Hand);
	if(IsValid(HandController))
		return  HandController->GetABPInstance();
	return nullptr;
}

UMotionControllerComponent* ALeeXRCharacter::GetMotionController(bool isLeft) const
{

	auto Hand = isLeft ? XRHandLeft : XRHandRight;
	if (IsValid(Hand))
	{
		if (auto Control = Hand->GetMotionController())
			return Control;
	}

	return nullptr;
}

void ALeeXRCharacter::CalculateMotionControllerVelocities()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	//Calculate Left Hand Velocity
	LeftHandVelocity = XRHandLeft->GetMCLocationToWorld() - LastFrameLeftHandLocation;
	LastFrameLeftHandLocation = LeftHandVelocity;

	//Calculate Right Hand Velocity
	RightHandVelocity = XRHandRight->GetMCLocationToWorld() - LastFrameRightHandLocation;
	LastFrameRightHandLocation = RightHandVelocity;

}

void ALeeXRCharacter::UpdateClimbing()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	FVector AccumulatedClimbVelocity = FVector::ZeroVector;

	if (IsValid(HeldLeftObject.Get()))
	{
		//Check if the player is climbing
		isClimbing = HeldLeftObject->ActorHasTag(TEXT("Climb"));
		
		AccumulatedClimbVelocity += FVector(0.0f, 0.0f, 0.0f);	// MCLeffVelocity
	}


	if (IsValid(HeldRightObject.Get()))
	{
		//Check if the player is climbing
		isClimbing = HeldRightObject->ActorHasTag(TEXT("Climb"));

		AccumulatedClimbVelocity += FVector(0.0f, 0.0f, 0.0f);	// MCLeffVelocity


		//GetCapsuleComponent()->AddWorldOffset(AccumulatedClimbVelocity*-1, true);

		LastFrameLeftHandLocation -= AccumulatedClimbVelocity;
		LastFrameRightHandLocation -= AccumulatedClimbVelocity;
		
	}
	//Check if the player is climbing


	UCapsuleComponent* Capsule = GetCapsuleComponent();
	
	Capsule->SetEnableGravity(!isClimbing);
	//ECollisionEnabled::Type CollisionType = isClimbing ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics;
	//Capsule->SetCollisionEnabled(CollisionType);

}

void ALeeXRCharacter::InitPhysicsContraints(bool isLeft)
{
	if (!IsValid(XRHandLeft) || !IsValid(XRHandRight)) return;

	if (auto Hand = isLeft ? XRHandLeft : XRHandRight)
	{
		if (IsValid(Hand)) {
			if (UPhysicsConstraintComponent* PhysicContraint = Hand->GetPhysicsContraint()) {
				USkeletalMeshComponent* HandPhys = Hand->IsHandLeft() ? HandPhysicsLeft : HandPhysicsRight;
				FString BoneName = Hand->IsHandLeft() ? "hand_l" : "hand_r";
				PhysicContraint->SetConstrainedComponents(Hand->GetHandSkeletal(), *BoneName, HandPhys, *BoneName);
			}
		}
	}
}

void ALeeXRCharacter::InitPhysicsAnimation(bool isLeft)
{
	if (!IsValid(AnimPhysicsLeft) || !IsValid(AnimPhysicsRight)) return;

	UPhysicalAnimationComponent* Anim = isLeft ? AnimPhysicsLeft : AnimPhysicsRight;
	FName SideName = isLeft ? TEXT("hand_l") : TEXT("hand_r");

	if (IsValid(Anim)) {

		FPhysicalAnimationData* AnimData = new FPhysicalAnimationData();
		AnimData->OrientationStrength = 10000.f;
		AnimData->PositionStrength = 1000.f;
		AnimData->VelocityStrength = 150.f;
		AnimData->AngularVelocityStrength = 150.f;

		USkeletalMeshComponent* Hand = isLeft ? HandPhysicsLeft : HandPhysicsRight;
		Anim->SetSkeletalMeshComponent(Hand);
		Anim->SetStrengthMultiplyer(1.0f);
		//LEE_LOG()
		Anim->ApplyPhysicalAnimationSettingsBelow(SideName, *AnimData);
	}
}

UAnimInstance* ALeeXRCharacter::GetPhysicsAnimInstance(bool isLeft)
{
	if (!IsValid(HandPhysicsLeft) || !IsValid(HandPhysicsRight)) return nullptr;

	return isLeft ? HandPhysicsLeft->GetAnimInstance() : HandPhysicsRight->GetAnimInstance();
}

void ALeeXRCharacter::SetPhysicsAllBodyBlendWeight(float inWeight,bool isLeft)
{
	//Set Up Bone Physisc
	FString BoneName = isLeft ? "hand_r" : "hand_l";
	USkeletalMeshComponent* Hand = isLeft ? HandPhysicsLeft : HandPhysicsRight;
	if (Hand) {
		Hand->SetAllBodiesBelowSimulatePhysics(*BoneName, true);
		Hand->SetAllBodiesBelowPhysicsBlendWeight(*BoneName, inWeight, false, true);
		LEE_LOG(LogLeeXRHandController, Log, "Set Physics All Body Blend Weight %f", inWeight);
	}
}

void ALeeXRCharacter::PauseHandPhysics(bool isEnable, bool isLeft)
{
	FString BoneName = isLeft ? "hand_r" : "hand_l";

	float Weight = isEnable ? .2f : 0.f;
	USkeletalMeshComponent* Hand = isLeft ? HandPhysicsLeft : HandPhysicsRight;
	ALeeXRHandBase* HandBase = isLeft ? XRHandLeft : XRHandRight;
	Hand->SetAllBodiesBelowSimulatePhysics(*BoneName, isEnable, true);
	Hand->SetAllBodiesBelowPhysicsBlendWeight(*BoneName, Weight);

	if (isEnable)
		Hand->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	else
		Hand->AttachToComponent(LeeXROrigin, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ALeeXRCharacter::HandPhysicBlendToPoseable(UPoseableMeshComponent* inPoseable, bool isLeft)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	if (inPoseable == nullptr) return;
	
	USkeletalMeshComponent* Hand = isLeft ? HandPhysicsLeft : HandPhysicsRight;

	//Set Physics Blend Weight
	TArray<FName> BoneNames = Hand->GetAllSocketNames();

	for (auto Bone : BoneNames)
	{
		FTransform BoneTrans = inPoseable->GetSocketTransform(Bone);
		auto BoneIdx = Hand->GetBoneIndex(Bone);
		Hand->GetBodyInstance(Bone)->SetBodyTransform(BoneTrans,ETeleportType::ResetPhysics);
		LEE_LOG(LogLeeXRHandController, Log, "Bone Name %s", *Bone.ToString());
	}
	Hand->SetVisibility(true);

}

EICTUActionType ALeeXRCharacter::GetCurrentActionType() const
{
	if (auto GameIns = GetGameInstance<ULeeXRGameInstance>())
	{
		return GameIns->GetActionType();
	}
	return EICTUActionType();
}

// Called when the game starts or when spawned
void ALeeXRCharacter::BeginPlay()
{
	Super::BeginPlay();
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	FString CurrentLevel = GetWorld()->GetMapName();

	XRHandLeft = HandInitialize(HandType, true);
	XRHandRight = HandInitialize(HandType, false);

	bool IsHome = CurrentLevel.EndsWith("HomeMenu");

	//Camera->bLockToHmd = !IsHome;

	//HandPhysicsLeft->SetHiddenInGame(!IsHome);
	//HandPhysicsRight->SetHiddenInGame(!IsHome);


	//Int Player Spawn Location
	if (auto GameIns = GetGameInstance<ULeeXRGameInstance>())
	{
		auto NextLoc = GameIns->GetLevelSpawnLocation();

		NextLevel = CurrentLevel.EndsWith("HomeMenu") ? NextLoc.SpawnLevel.Num() - 1 : GameIns->CurrentLevel;

		if (NextLoc.SpawnLevel.Num() > 0 && NextLevel >= 0)
		{
			FVector StartSpawnLocation = NextLoc.SpawnLevel[NextLevel];
			SetActorLocation(StartSpawnLocation);
		}
	}

	ADDMEMORYSTAT(this, STAT_ICTUCharacterMemory);
	InitializationContext(GetWorld(), DefaultMappingContext);
	InitializationContext(GetWorld(), HandMappingContext, 1);



	//Init VR Origin
	InitVRTrackingOrigin();


	//init COntraints
	//InitPhysicsContraints();

	//HandPhysicsRight->SetAllBodiesBelowSimulatePhysics(TEXT("hand_r"), true);
	//HandPhysicsLeft->SetAllBodiesBelowSimulatePhysics(TEXT("hand_l"), true);

	//InitPhysicsAnimation();

	LEE_LOG(LogLeeXRCharacter, Log, "Begin Play");
	//HandPhysicsRight->SetAllBodiesBelowPhysicsBlendWeight(TEXT("hand_r"), .15f);
}

void ALeeXRCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	///Reset Profile Memories
	SET_MEMORY_STAT(STAT_ICTUCharacter, 0);

	MemoriesSize = 0;
	SET_MEMORY_STAT(STAT_ICTUMV_TotalMemories, 0);

}

void ALeeXRCharacter::OnHMDOrientReset()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALeeXRCharacter::OnHMDLevelChanged_Implementation(const FString& NewLevelName)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	//Set Tracking Origin to FLoor
	InitVRTrackingOrigin();

}

#if WITH_EDITOR
void ALeeXRCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	if (PropertyChangedEvent.Property)
	{
		FName PropertyName = PropertyChangedEvent.Property->GetFName();
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ALeeXRCharacter, HandType))
		{
			if (HandType == ELeeXRHandType::LeeXRHandTracking)
			{
				//Do Something With Hand Tracking Editor Mode
			}

		}
	}
}
#endif
// Called every frame
void ALeeXRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	///Climbing Option
	/*{
		CalculateMotionControllerVelocities();
		UpdateClimbing();
	}*/
}

// Called to bind functionality to input
void ALeeXRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind the action to the delegate
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Started, this, &ALeeXRCharacter::OnMoving);

		EnhancedInputComponent->BindAction(IA_RMenuToogle, ETriggerEvent::Started, this, &ALeeXRCharacter::OnResetOrientation);
		EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Started, this, &ALeeXRCharacter::OnTurn);

		//EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Started, this, &ALeeXRCharacter::OnHandGrabing);
		//EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnHandRelease);
		//EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnHandRelease);
		//EnhancedInputComponent->BindAction(IA_GraspLeft, ETriggerEvent::Triggered, this, &ALeeXRCharacter::OnHandTrigger);


		//EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Started, this, &ALeeXRCharacter::OnHandGrabing);
		//EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnHandGrabing);
		//EnhancedInputComponent->BindAction(IA_Grasp, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnHandGrabing);

		//EnhancedInputComponent->BindAction(IA_RMenuInteract, ETriggerEvent::Started, this, &ALeeXRCharacter::OnHandInteract);
		//EnhancedInputComponent->BindAction(IA_RMenuInteract, ETriggerEvent::Canceled, this, &ALeeXRCharacter::OnHandInteract);
		//EnhancedInputComponent->BindAction(IA_RMenuInteract, ETriggerEvent::Completed, this, &ALeeXRCharacter::OnHandInteract);

	}

}

// Called when the player is moving
void ALeeXRCharacter::OnMoving()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);


	//FVector StartPos = XRHandRight->GetMotionControllerLocation();
	//FVector ForwardVec = XRHandRight->GetMotionControllerForwardVector();

	//TeleportTrace(StartPos, ForwardVec);

	//UKismetSystemLibrary::DrawDebugSphere(this, ProjectedTeleportLocation, 10.0f, 12, FColor::Green, 5.0f, 0.0f);

	//LeeScreenLog("Location %s", FColor::Cyan,*ProjectedTeleportLocation.ToString());

}

/// <summary>
/// Grab Controller Input 
/// </summary>
/// <param name="ActionInstance"></param>
void ALeeXRCharacter::OnHandGrabing(const FInputActionInstance& ActionInstance)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	FString ActName = ActionInstance.GetSourceAction()->GetName();

	ALeeXRHandController* HandController = Cast<ALeeXRHandController>(XRHandRight);

	if (!HandController) return;

	if (HandController->IsValidControllerType(ELeeXRHandType::LeeXRController))
		HandController->OnGrabObject();

	ULeeXRAnimInstance* LeeAnimIns = Cast<ULeeXRAnimInstance>(HandController->GetABPInstance());
	if (LeeAnimIns)
	{
		ETriggerEvent inEventType = ActionInstance.GetTriggerEvent();
		LeeAnimIns->PoseAlphaGrasp = inEventType == ETriggerEvent::Started ? 1.0f : 0.0f;
	}
}

void ALeeXRCharacter::OnResetOrientation()
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
	//Menu

	LeeScreenLog("Reset Orientation", FColor::Green);
}

void ALeeXRCharacter::OnTurn(const FInputActionInstance& ActionInstance)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);

	LeeScreenLog("Turn", FColor::Green);
	//FRotator CurrentRotation = GetActorRotation();
	//FRotator NewRotation = CurrentRotation + FRotator(0.0f, 90.0f, 0.0f);
	//SetActorRotation(NewRotation);
}

// Initialize the hands
ALeeXRHandBase* ALeeXRCharacter::HandInitialize(ELeeXRHandType inType,bool isLeft)
{
	LEE_SCOPE_CYCLE_COUNTER(ICTUCharacter);
	if (DataLeft == nullptr || DataRight == nullptr)
	{
		LeeScreenLog("Hand Data Not Found", FColor::Red);
		return nullptr;
	}
	
	ULeeXRHandDataAsset* Data = isLeft ? DataLeft : DataRight;
	USkeletalMeshComponent* HandPhysics = isLeft ? HandPhysicsLeft : HandPhysicsRight;
	FName HandName = isLeft ? "hand_l" : "hand_r";
	// Initialize the hand actor
	switch (inType)
	{
		case ELeeXRHandType::LeeXRController: {
			return InitializeHandActor<ALeeXRHandController>(Data->Assets.Controller);
		}
		case ELeeXRHandType::LeeXRHandTracking: {
			return InitializeHandActor<ALeeXRHandTracking>(Data->Assets.Tracking);
		}
		case ELeeXRHandType::LeeXRHandPhysics: {
			return InitializeHandActor<ALeeXRHandPhysics>(Data->Assets.Physics);
		}
	}


	InitPhysicsContraints(isLeft);

	HandPhysics->SetAllBodiesBelowSimulatePhysics(HandName, true);

	InitPhysicsAnimation(isLeft);

	return nullptr;

}

void ALeeXRCharacter::InitSpawnLocation()
{

}

void ALeeXRCharacter::InitVRTrackingOrigin()
{
	//Set Tracking Origin to FLoor
	bool isEnable = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	if (isEnable)
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::LocalFloor);
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("vr.PixelDensity 1.0"));

	}

}


#undef LOCTEXT_NAMESPACE
