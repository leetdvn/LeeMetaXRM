// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LeeXRMeshSocket.h"
#include "Materials/MaterialInstance.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"

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

	if (OnComponentBeginOverlap.IsBound()) OnComponentBeginOverlap.Clear();

	//OnComponentBeginOverlap.AddDynamic(this, &ULeeXRMeshSocket::OnBeginOverlap);
	//OnComponentEndOverlap.AddDynamic(this, &ULeeXRMeshSocket::OnEndOverlap);

	if (!TargetSocketName.IsEmpty()) AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, *TargetSocketName);

}

void ULeeXRMeshSocket::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (SweepResult.HasValidHitObjectHandle())
	{
	}
}

void ULeeXRMeshSocket::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Reset Color
	MaterialIns->SetScalarParameterValue(*MaterialParamName, DefaultParamValue);
}
