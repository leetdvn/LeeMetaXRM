// Fill out your copyright notice in the Description page of Project Settings.


#include "APawn/LeeXRPawn.h"
#include <EnhancedInputComponent.h>
#include <Camera/CameraComponent.h>
#include <MotionControllerComponent.h>
#include <VRNotificationsComponent.h>
#include <Components/WidgetComponent.h>
#include <HeadMountedDisplayFunctionLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "Common/Definitions.h"
#include <XRMotionControllerBase.h>
#include <OculusXRHandComponent.h>
#include <SphereComponent.h>
#include "Components/LeeXRSphereComponent.h"
#include "Components/LeeXRGrabComponent.h"
#include <EnhancedInputSubsystems.h>
#include <NiagaraComponent.h>
#include <NavigationSystem.h>
#include "NiagaraFunctionLibrary.h"

// Sets default valuess
ALeeXRPawn::ALeeXRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/**Init Components **/
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("LeeVROrigin"));
	RootComponent = VROrigin;

	LCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("XRCamera"));
	LCamera->SetupAttachment(RootComponent);

	HeadMountedDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMountedDisplayMesh"));
	HeadMountedDisplayMesh->SetupAttachment(LCamera);

	/*Left Grip*/
	LeftGrip = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_LeftGrip"));
	LeftGrip->SetupAttachment(RootComponent);

	XRDeviceLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XR_LeftDevice"));
	XRDeviceLeft->SetupAttachment(LeftGrip);

	//HandTrackLeft = CreateDefaultSubobject<UOculusXRHandComponent>(TEXT("HandTrackLeft"));
	//HandTrackLeft->SetupAttachment(LeftGrip);
	/*Right Grip*/
	RightGrip = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_RightGrip"));
	RightGrip->SetupAttachment(RootComponent);

	XRDeviceRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XR_RightDevice"));
	XRDeviceRight->SetupAttachment(RightGrip);

	//HandTrackRight = CreateDefaultSubobject<UOculusXRHandComponent>(TEXT("HandTrackRight"));
	//HandTrackRight->SetupAttachment(RightGrip);

	LeftAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_LeftAim"));
	LeftAim->SetupAttachment(RootComponent);

	WidgetInteractionLeft = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetInteractionLeft"));
	WidgetInteractionLeft->SetupAttachment(LeftAim);

	RightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_RightAim"));
	RightAim->SetupAttachment(RootComponent);

	WidgetInteractionRight = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetInteractionRight"));
	WidgetInteractionRight->SetupAttachment(RightAim);

	Notify = CreateDefaultSubobject<UVRNotificationsComponent>(TEXT("VRNotification"));


	//IndexLeftCollison = CreateDefaultSubobject<ULeeXRSphereComponent>(TEXT("IndexLeftCollison"));

	//IndexLeftCollison->AttachToComponent(HandTrackLeft.Get(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Index3"));

	//ThumbLeftCollison = CreateDefaultSubobject<ULeeXRSphereComponent>(TEXT("ThumbLeftCollison"));
	//ThumbLeftCollison->AttachToComponent(HandTrackLeft.Get(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Thumb3"));


	//IndexRightCollison = CreateDefaultSubobject<ULeeXRSphereComponent>(TEXT("IndexRightCollison"));

	//IndexRightCollison->AttachToComponent(HandTrackRight.Get(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Index3"));

	//ThumbRightCollison = CreateDefaultSubobject<ULeeXRSphereComponent>(TEXT("ThumbRightCollison"));
	//ThumbRightCollison->AttachToComponent(HandTrackRight.Get(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Thumb3"));

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

	InitializeComponents();
}

void ALeeXRPawn::StartTeleportTrace()
{
	UE_LOG(LeeXRCharacters, Warning, TEXT("Teleport Trace Started"));

	bTeleportTraceActive = true;

	NiagaraComponent->SetVisibility(true);

	if (ActorToSpawn == nullptr) return;

	AActor* ActorSpawn= GetWorld()->SpawnActor<AActor>(ActorToSpawn->StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	
	//if (ActorSpawn !=nullptr)
	//{
	//	ActorToSpawn = ActorSpawn->GetClass();
	//}

}

bool ALeeXRPawn::IsValidTeleportLocation(FHitResult Hit, FVector& ProjectedLocation)
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!NavSys) return false;


	return  NavSys->K2_ProjectPointToNavigation(
		GetWorld(),
		Hit.Location,
		ProjectedLocation,
		NavSys->GetAbstractNavData(),
		NULL,
		TeleportProjectPointToNavigationQueryExtent);
}

void ALeeXRPawn::TeleportTrace(FVector StartPos, FVector ForwardVec)
{
	//Trace Teleport
	LEE_LOG(LeeXRCharacters, Log, "Trace Teleport");
	LeeScreenLog("Trace Teleport", FColor::Green);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	float teleportSpped = 650.0f;
	float TeleportRadius = 3.6f;
	float LocalNavMeshCellHeight = 8.0f;
	FHitResult OutHit{};
	TArray<FVector> PathPositions = TArray<FVector>();
	FVector LastTraceDestination = FVector::ZeroVector;

	UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(
		GetWorld(),
		OutHit,
		PathPositions,
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
	PathPositions.Insert(StartPos, 0);
	FVector ProjectedLocation{};
	bool isTeleportValid = IsValidTeleportLocation(OutHit, ProjectedLocation);

	ProjectedTeleportLocation = FVector(ProjectedLocation.X, ProjectedLocation.Y, ProjectedLocation.Z- 8.0f);

	if (bValidTeleportLocation != isTeleportValid)
	{
		bValidTeleportLocation = isTeleportValid;
		ActorToSpawn.GetDefaultObject()->GetRootComponent()->SetVisibility(bValidTeleportLocation);
	}

	//**Then Update Sequence 2*//
	//ActorToSpawn.GetDefaultObject()->SetActorLocation(ProjectedTeleportLocation);
	//Nagra
	/*UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NiagaraComponent,
		TEXT("User.PointArray"),
		PathPositions);*/
}

void ALeeXRPawn::EndTeleportTrace()
{
	LeeScreenLog("End Trace Teleport", FColor::Green);
}

void ALeeXRPawn::TryTeleport()
{
	LeeScreenLog("Try Teleport", FColor::Green);
}

void ALeeXRPawn::IAMove(const FInputActionInstance& ActionInstance)
{
	///Trace Teleport
	LeeScreenLog("Move Trigger %s", FColor::Cyan);
	
	FVector StartPos = RightAim->K2_GetComponentToWorld().GetLocation();
	FVector ForwardVec= RightAim->GetForwardVector();
	TeleportTrace(StartPos, ForwardVec);

}

void ALeeXRPawn::IAMoveStart(const FInputActionInstance& ActionInstance)
{

	LeeScreenLog("Move Start", FColor::Green);

	StartTeleportTrace();

}

void ALeeXRPawn::IAMoveComplete(const FInputActionInstance& ActionInstance)
{
	LeeScreenLog("End Trace Teleport", FColor::Green);

	EndTeleportTrace();
}

void ALeeXRPawn::IATurnStart()
{
	/*Turn Start*/
	LeeScreenLog("Turn Start", FColor::Green);

	bStartLine=  IndexLeftCollison->K2_GetComponentToWorld().GetLocation();


}

void ALeeXRPawn::IATurnCompleted()
{
	/*Turn Compoleted*/
	LeeScreenLog("Turn Completed", FColor::Green);

	bEndLine = IndexLeftCollison->K2_GetComponentToWorld().GetLocation();
	UKismetSystemLibrary::DrawDebugLine(GetWorld(),
		bStartLine,
		bEndLine,
		FColor::Green,
		5,
		5
	);

}

void ALeeXRPawn::IAGrabLeftStart()
{
	//LeftGrip->GetGrap
	ULeeXRGrabComponent* Grab = GetGrapComponentNearController(LeftGrip);

	if (IsValid(Grab))
	{
		if (Grab->TryGrab(LeftGrip)) {
			if (HeldComponentLeft == HeldComponentRight)
			{
				HeldComponentRight = nullptr;
			}
		}
	}
}

void ALeeXRPawn::IAGrabLeftCompleted()
{
	LeeScreenLog("Grab Completed", FColor::Green);
	ULeeXRGrabComponent* Grab = GetGrapComponentNearController(LeftGrip);

	if (IsValid(Grab))
	{
		if (Grab->TryRelease())
		{
			HeldComponentLeft = nullptr;
		}
	}
	//TryRelease();
}

void ALeeXRPawn::IAGrabRightStart()
{
	LeeScreenLog("Grab Right", FColor::Green);
}

void ALeeXRPawn::IAGrabRightCompleted()
{
	LeeScreenLog("Grab Right Completed", FColor::Green);
}

void ALeeXRPawn::IALMenuToogle(const FInputActionInstance& ActionInstance)
{
	const UInputAction* SourceAction = ActionInstance.GetSourceAction();

	bool isLeft = SourceAction->GetName().EndsWith("Left") ?
		true : false;
	//LeeScreenLog<FString>(SourceAction->GetName(), FColor::Green);
	if (isLeft) { LeeScreenLog("Menu Toogle Left", FColor::Green); }
	else { LeeScreenLog("Menu Toogle Right", FColor::Green); }

	ToogleMenu(isLeft);


	//UOculusXRHandComponent* Hand = isLeft ? HadnTra : RightHand;
}

void ALeeXRPawn::IARMenuToogle()
{
	LeeScreenLog("Menu Toogle", FColor::Green);
}

ULeeXRGrabComponent* ALeeXRPawn::GetGrapComponentNearController(UMotionControllerComponent* MotionController)
{
	//*Grab Left*//


	FVector LocalGripPosition = MotionController->K2_GetComponentToWorld().GetLocation();
	ULeeXRGrabComponent* LocalGrabComponents = nullptr;
	TArray<AActor*> IgnoreActors = TArray<AActor*>();
	FHitResult OutHit{};
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	IgnoreActors.Add(this);

	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		LocalGripPosition,
		LocalGripPosition,
		GrabRadiusFromGribPosition, // Sphere radius
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true,
		FLinearColor::Green,
		FLinearColor::Red,
		5.0f
	);


	if (bHit)
	{
		double LocalNearestComponentDistance = 9999999.0;
		TArray<ULeeXRGrabComponent*> Grabs = TArray<ULeeXRGrabComponent*>();
		OutHit.GetActor()->GetComponents<ULeeXRGrabComponent>(Grabs, true);
		if (Grabs.Num() > 0)
		{
			for (auto Grab : Grabs)
			{
				FVector WorldLoc = Grab->K2_GetComponentToWorld().GetLocation();
				double lengthSquare = UKismetMathLibrary::VSizeSquared(WorldLoc - LocalGripPosition);

				if (lengthSquare <= LocalNearestComponentDistance) {
					LocalNearestComponentDistance = lengthSquare;
					return Grab;
				}

			}
		}

	}


	return LocalGrabComponents;
}

void ALeeXRPawn::OnHitComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	LeeScreenLog("Hit Info : %s", FColor::Green, *HitComponent->GetName());
}

// Called when the game starts or when spawned
void ALeeXRPawn::BeginPlay()
{
	Super::BeginPlay();
	
	//Set Tracking Origin to FLoor
	bool isEnable = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();

	if (isEnable)
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("vr.PixelDensity 1.0"));
	}

	//IndexLeftCollison->OnComponentHit.AddDynamic(this, &ALeeXRPawn::OnHitComponent);
	//ThumbLeftCollison->OnComponentHit.AddDynamic(this, &ALeeXRPawn::OnHitComponent);

}

// Called every frame
void ALeeXRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALeeXRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Bind the action to the delegate
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ALeeXRPawn::IAMove);

		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Started, this, &ALeeXRPawn::IAMoveStart);

		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &ALeeXRPawn::IAMoveComplete);

		EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Started, this, &ALeeXRPawn::IATurnStart);

		EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Completed, this, &ALeeXRPawn::IATurnCompleted);

		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Started, this, &ALeeXRPawn::IAGrabLeftStart);
		
		EnhancedInputComponent->BindAction(IA_GrabLeft, ETriggerEvent::Completed, this, &ALeeXRPawn::IAGrabLeftCompleted);

		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Started, this, &ALeeXRPawn::IAGrabRightStart);

		EnhancedInputComponent->BindAction(IA_GrabRight, ETriggerEvent::Completed, this, &ALeeXRPawn::IAGrabRightCompleted);

		EnhancedInputComponent->BindAction(IA_LMenuToogle, ETriggerEvent::Started, this, &ALeeXRPawn::IALMenuToogle);

		EnhancedInputComponent->BindAction(IA_RMenuToogle, ETriggerEvent::Started, this, &ALeeXRPawn::IALMenuToogle);

		LeeScreenLog("Setup Player Input", FColor::Green);

	}
}

void ALeeXRPawn::ToogleMenu(bool isLeft)
{
}

void ALeeXRPawn::InitializeComponents()
{
	LeftGrip->MotionSource = TEXT("Left");
	RightGrip->MotionSource = TEXT("Right");

	//HandTrackLeft->SkeletonType = HandTrackLeft->MeshType = EOculusXRHandType::HandLeft;
	//HandTrackRight->SkeletonType = HandTrackRight->MeshType = EOculusXRHandType::HandRight;
}

void ALeeXRPawn::InitializeMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			//Subsystem->AddMappingContext(HandMappingContext.LoadSynchronous(), 0);
		}
	}
}

