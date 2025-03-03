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
	if (SweepResult.HasValidHitObjectHandle())
	{
		MaterialIns->SetScalarParameterValue(*MaterialParamName,1.f);
		ALeeXRGrabActors* GrabActor = Cast<ALeeXRGrabActors>(OtherActor);
		if (GrabActor &&  GrabActor->IsTag(SocketTag))
		{
			GrabActor->SetSimulation(false);
			OtherActor->AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetAttachSocketName());
			SetVisibility(false);
			LeeScreenLog("P :%s Other : %s", FColor::Red,*GetAttachParent()->GetName(),*OtherActor->GetName());

		}
	}
}

void ULeeXRMeshSocket::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Reset Color
	MaterialIns->SetScalarParameterValue(*MaterialParamName, DefaultParamValue);
}
