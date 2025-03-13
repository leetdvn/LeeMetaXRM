// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LeeXRMeshSocket.h"
#include "Materials/MaterialInstance.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Actors/LeeXRGrabActors.h"
#include "LeeXRUltils.h"

using namespace LeeXRUltils;


ULeeXRMeshSocket::ULeeXRMeshSocket(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void ULeeXRMeshSocket::ConstructionEditor()
{

	if (!GetStaticMesh()) return;

	MaterialIns = CreateDynamicMaterialInstance(0, GetMaterial(0), TEXT("LeeXRMaterialDynamic"));
		
	if (MaterialIns) {
		SetMaterialInstanceStaticSwitchParameterValue(MaterialIns, TEXT("OnOff"), true);
	}
	
}

void ULeeXRMeshSocket::SetFlicker(bool isTurnOn)
{
	if (MaterialIns == nullptr) return;

	float isFlicker = isTurnOn ? 1.f : 0.f;
	MaterialIns->SetScalarParameterValue(*MaterialParamName, isFlicker);
}

void ULeeXRMeshSocket::SetCorrectShape(bool isTrue)
{
	if (MaterialIns == nullptr) return;

	float isCorrect = isTrue ? 1.f : 0.f;
	MaterialIns->SetScalarParameterValue(TEXT("TrueColor"), isCorrect);

}

void ULeeXRMeshSocket::BeginPlay()
{
	Super::BeginPlay();

	MaterialIns = CreateAndSetMaterialInstanceDynamic(0);

	//Bind Event Overlapped
	if (OnComponentBeginOverlap.IsBound()) OnComponentBeginOverlap.Clear();
	OnComponentBeginOverlap.AddDynamic(this, &ULeeXRMeshSocket::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &ULeeXRMeshSocket::OnEndOverlap);

}

///Overlapped Event
void ULeeXRMeshSocket::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	USphereComponent* SphereCollision = Cast<USphereComponent>(OtherComp);
	if (SphereCollision) return;


	///Check if the object is a grabbable object
	if (SweepResult.HasValidHitObjectHandle())
	{
		if (auto* GrabActor = Cast<ALeeXRGrabActors>(OtherActor)) {
			SetFlicker(true);
			
			///Check Tag Element for Snapable Object
			if (GrabActor && GrabActor->IsTag(SocketTag,false))
			{
				SetCorrectShape(true);
				//if (!GrabActor->IsSimulation()) return;
				OnMeshSocketUpdate.Broadcast();
				{
					GrabActor->SetSimulation(false);
					//GrabActor->SetFreeze(GrabActor->IsFreeze());
					OtherActor->AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetAttachSocketName());
					OtherActor->SetActorRelativeLocation(FVector::ZeroVector);
					SetFlicker(false);

				}

			}
			else {
				SetCorrectShape(false);
			}
		}

	}
}

void ULeeXRMeshSocket::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Reset Color
	SetFlicker(false);
}
