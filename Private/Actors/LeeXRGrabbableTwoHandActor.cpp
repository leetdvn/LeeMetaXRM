// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRGrabbableTwoHandActor.h"

ALeeXRGrabbableTwoHandActor::ALeeXRGrabbableTwoHandActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RootSkeletal"));
	SetRootComponent(RootSkeletal);

	MainGribCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MainGribCollision"));
	MainGribCollision->SetupAttachment(RootSkeletal);

	SecondaryGribCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SecondaryGribCollision"));
	SecondaryGribCollision->SetupAttachment(RootSkeletal);

}

void ALeeXRGrabbableTwoHandActor::InitSettings()
{
	Super::InitSettings();


	FName CollisionProfile = GrabableType == ELeeXRGrabableType::LeeXROneHand ? TEXT("NoCollision") : TEXT("Grabbable");

	if (!Sockets.MainSocketName.IsEmpty())
		MainGribCollision->AttachToComponent(RootSkeletal, FAttachmentTransformRules::SnapToTargetIncludingScale, *Sockets.MainSocketName);

	if (!Sockets.SecondarySocketName.IsEmpty())
		SecondaryGribCollision->AttachToComponent(RootSkeletal, FAttachmentTransformRules::SnapToTargetIncludingScale, *Sockets.SecondarySocketName);

	//MainGribCollision->SetCollisionProfileName(CollisionProfile);
	//SecondaryGribCollision->SetCollisionProfileName(CollisionProfile);
	//MainGribCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//SecondaryGribCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);


}

void ALeeXRGrabbableTwoHandActor::BeginPlay()
{
	Super::BeginPlay();
}

void ALeeXRGrabbableTwoHandActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(MainController) && IsValid(SecondaryController))
	{
		FRotator TwoHandRotation = GetTwoHandRotation(MainController, SecondaryController);
		SetActorRotation(TwoHandRotation);
	}
}

#if WITH_EDITOR
void ALeeXRGrabbableTwoHandActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	InitSettings();
}

void ALeeXRGrabbableTwoHandActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PROPERTYCHANGED(ALeeXRGrabbableTwoHandActor,Sockets))
	{
		InitSettings();
	}
}
#endif
void ALeeXRGrabbableTwoHandActor::OnGrab(UObject* inComponent, const FVector& InGrabLocation)
{
}

void ALeeXRGrabbableTwoHandActor::OnRelease(UObject* inComponent)
{
}

void ALeeXRGrabbableTwoHandActor::OnGrabObjects(UMotionControllerComponent* inComponent)
{
	auto MCLoc = LeeXRGetWorldLocation(inComponent);
	auto MainLoc = LeeXRGetWorldLocation(MainGribCollision);
	auto SecondaryLoc = LeeXRGetWorldLocation(SecondaryGribCollision);

	float MainDist = FVector::Dist(MCLoc, MainLoc);

	float SecondaryDist = FVector::Dist(MCLoc, SecondaryLoc);

	float MainSphere = MainGribCollision->GetScaledSphereRadius();

	float SecondarySphere = SecondaryGribCollision->GetScaledSphereRadius();


	if (MainDist <= MainSphere) {
		MainController = inComponent;

		//Attach the Actor to the Controller
		RootSkeletal->K2_AttachToComponent(
			inComponent, 
			NAME_None,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, true);
		//Set the Socket
		SetActorRelativeTransform(RootSkeletal->GetSocketTransform(*Sockets.MainSocketName, ERelativeTransformSpace::RTS_Actor));

	}
	else {
		if (SecondaryDist <= SecondarySphere) 
		{
			//Attach the Actor to the Controller  
			SecondaryController = inComponent;
		}

	}

}

void ALeeXRGrabbableTwoHandActor::OnReleaseObjects(UMotionControllerComponent* inComponent)
{
	if (inComponent == nullptr) return;


	if (MainController == inComponent)
	{
		MainController = nullptr;
		SecondaryController = nullptr;
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (SecondaryController == inComponent)
	{
		SecondaryController = nullptr;
		if (IsValid(MainController)) {
			///Socket Mant Socket
			SetActorRelativeTransform(RootSkeletal->GetSocketTransform(*Sockets.SecondarySocketName, ERelativeTransformSpace::RTS_Actor));
		}
		else {
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}

}

