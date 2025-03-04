// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRHandTracking.h"
#include "OculusXRHandComponent.h"
#include "HandPoseRecognizer.h"
#include "Common/LeeXRUltils.h"


using namespace LeeXRUltils;

void ALeeXRHandTracking::TickUntilGrasp(const UObject* WorldContextObject, ELeeTickUntilInputPin InputPin, FLatentActionInfo LatentInfo)
{
	return TickUntil(WorldContextObject, InputPin, LatentInfo);
}


ALeeXRHandTracking::ALeeXRHandTracking(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HandTrackingComp = CreateDefaultSubobject<UOculusXRHandComponent>(TEXT("HandTrackingComp"));
	HandTrackingComp->SetupAttachment(MotionController);

	NiagaraComponent->SetupAttachment(HandTrackingComp);

	ControllerType = ELeeXRHandType::LeeXRHandTracking;

	SphereIndex = CreateDefaultSubobject<USphereComponent>(TEXT("SphereIndex"));
	SphereIndex->SetupAttachment(HandTrackingComp);

	SphereThumb = CreateDefaultSubobject<USphereComponent>(TEXT("SphereThumb"));
	SphereThumb->SetupAttachment(HandTrackingComp);

	HandPoseRecognizer = CreateDefaultSubobject<UHandPoseRecognizer>(TEXT("HandPoseRecognizer"));
	HandPoseRecognizer->SetupAttachment(HandTrackingComp);
	WidgetInteraction->SetupAttachment(SphereIndex);

}



/// <summary>
/// Grasp Object
void ALeeXRHandTracking::GraspObject()
{
	//Grasp Object Hand Tracking
	CurrentGrabObject = TScriptInterface<ILeeXRInteraction>(HitThumbActor);
	if (CurrentGrabObject) {
		bIsHeld = true;

		FVector GrabLocation = HandTrackingComp->GetComponentLocation();
		CurrentGrabObject->OnGrab(HandTrackingComp, GrabLocation);
	}

	///Timm Distance Count
	if (TimerDelegate.IsBound()) TimerDelegate.Unbind();

	TimerDelegate.BindLambda([this]() {TrackingGrasp(); });

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.02f, true);

}

void ALeeXRHandTracking::GraspRelease()
{
	//Grasp Release Hand Tracking
	if (CurrentGrabObject == nullptr)	return;

	CurrentGrabObject->OnRelease(HandTrackingComp);
	CurrentGrabObject = nullptr;
	bIsHeld = false;

	if (TimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void ALeeXRHandTracking::BeginPlay()
{
	Super::BeginPlay();

	LEE_SCOPE_CYCLE_COUNTER(ICTUController);

	INC_MEMORY_STAT_BY(STAT_HandController, this->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal));

	if (SphereIndex->OnComponentBeginOverlap.IsBound()) SphereIndex->OnComponentBeginOverlap.Clear();
	SphereIndex->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRHandTracking::OnBeginOverlap);

	if (SphereThumb->OnComponentBeginOverlap.IsBound()) SphereThumb->OnComponentBeginOverlap.Clear();
	SphereThumb->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRHandTracking::OnBeginOverlap);

	//if (SphereIndex->OnComponentEndOverlap.IsBound()) SphereIndex->OnComponentEndOverlap.Clear();
	//SphereIndex->OnComponentEndOverlap.AddDynamic(this, &ALeeXRHandTracking::OnEndOverlap);

	//if (SphereThumb->OnComponentEndOverlap.IsBound()) SphereThumb->OnComponentEndOverlap.Clear();
	//SphereThumb->OnComponentEndOverlap.AddDynamic(this, &ALeeXRHandTracking::OnEndOverlap);
}

void ALeeXRHandTracking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHeld) return;

	if (HandPoseRecognizer && !HandPoseRecognizer->Poses.IsEmpty()) {

		FString Name;
		int Index;
		float Duration;
		float Error;
		float Confidence;
		HandPoseRecognizer->GetRecognizedHandPose(Index, Name, Duration, Error, Confidence);
		
		LeeHandPose hPose = LeeXRGetEnumValueByString<LeeHandPose>(Name);

		if (Name.EndsWith("None")) return;

		switch (hPose)
		{
		case LeeHandPose::LHandMenu:
			//Do Something
			break;
		case LeeHandPose::LHandGrasp:
			//if(!bIsHeld)
				//GraspObject();
			if (bTeleportTraceActive) {
				bTeleportTraceActive = false;
				TryTeleport();
			}
			break;
		case LeeHandPose::LHandRelease:
				//GraspRelease();
			break;
		case LeeHandPose::LHandMove:
			//Do Something
			if (HandType == EControllerHand::Right) {
				//Do Something
				bTeleportTraceActive = true;
				FVector StartPos = HandTrackingComp->GetComponentToWorld().GetLocation();
				FVector ForwardVec = NiagaraComponent->GetForwardVector();
				TeleportTrace(StartPos,ForwardVec);
			}
			break;
		}

	}
}

void ALeeXRHandTracking::InittializeSetup()
{
	Super::InittializeSetup();


	FTimerHandle AttachHandle;
	GetWorld()->GetTimerManager().SetTimer(AttachHandle, [this]() {
			this->AttachOculusHandTracking(EOculusXRBone::Index_Tip, this->SphereIndex);
			this->AttachOculusHandTracking(EOculusXRBone::Thumb_Tip, this->SphereThumb);
			NiagaraComponent->SetupAttachment(HandTrackingComp, *UOculusXRInputFunctionLibrary::GetBoneName(EOculusXRBone::Wrist_Root));
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

		},1.f, false, 1.f);

	
	SphereIndex->SetSphereRadius(1.f);
	SphereThumb->SetSphereRadius(1.f);
}

void ALeeXRHandTracking::OnBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LeeScreenLog("Overlap %s", FColor::Green, *OtherActor->GetName());
	HitIndexActor = OtherActor;
	HitThumbActor = OtherActor;


	//TArray<AActor*> OverlappingActors;
	//OtherComp->GetOverlappingActors(OverlappingActors);

	if (HitIndexActor == HitThumbActor)
		GraspObject();
}


void ALeeXRHandTracking::OnEndOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//LeeScreenLog("End Overlap %s", FColor::Red, *OtherActor->GetName());
}

void ALeeXRHandTracking::AttachOculusHandTracking(const EOculusXRBone inEbone,USphereComponent* inSphere)
{
	FString BoneName = UOculusXRInputFunctionLibrary::GetBoneName(inEbone);
	if (HandTrackingComp->DoesSocketExist(*BoneName)) {
		inSphere->AttachToComponent(HandTrackingComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *BoneName);
		inSphere->SetSphereRadius(1.f);
		float offset = HandType == EControllerHand::Right ? 1.0f : -1.0f;
		inSphere->SetRelativeLocation(FVector(offset, 0.0f, 0.0f));
	}
}

bool ALeeXRHandTracking::IsGrabable(TArray<AActor*> inOverlapActors, AActor* InArrayAct)
{
	if (inOverlapActors.IsEmpty()) return false;
	
	for (AActor* actor : inOverlapActors) {
		if (actor == InArrayAct) return true;
	}
	return false;
}

/// <summary>
/// Tracking Grasp ========================================
/// </summary>
void ALeeXRHandTracking::TrackingGrasp()
{
	float dist = FVector::Dist(SphereIndex->GetComponentToWorld().GetLocation(),
		SphereThumb->GetComponentToWorld().GetLocation());

	if (dist >= 4.5f) {
		GraspRelease();
	}
}