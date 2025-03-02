// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LeeXRMeshSocket.h"
#include "Materials/MaterialInstanceConstant.h"

ULeeXRMeshSocket::ULeeXRMeshSocket(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

#include "Materials/MaterialInstanceDynamic.h"

void ULeeXRMeshSocket::BeginPlay()
{
	Super::BeginPlay();

	MaterialIns = CreateAndSetMaterialInstanceDynamic(0);

	if (MaterialIns) {

		if (MaterialParamName.IsEmpty()) {
			FString FunctionName = __FUNCTION__;
			UE_LOG(LogTemp, Error, TEXT("Material Param Name is Empty %s"),*FunctionName);
		}
		// Set Material Opacity Value 0
		MaterialIns->SetScalarParameterValue(*MaterialParamName, 0.0f);
	}

	if (OnComponentBeginOverlap.IsBound()) OnComponentBeginOverlap.Clear();

	OnComponentBeginOverlap.AddDynamic(this, &ULeeXRMeshSocket::OnBeginOverlap);

	if (!TargetSocketName.IsEmpty()) AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, *TargetSocketName);
}

void ULeeXRMeshSocket::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (SweepResult.HasValidHitObjectHandle())
	{
		UStaticMeshComponent* HitMesh = Cast<UStaticMeshComponent>(OtherComp);
		if (HitMesh && HitMesh->IsSimulatingPhysics())
		{
			//Snap to Target

			OtherActor->AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetAttachSocketName());
			//Change Visibility
			SetVisibility(false);

		}
		else {
			//set Color to Green Correct to Snap
			MaterialIns->SetScalarParameterValue(*MaterialParamName, 1.0f);

		}
	}
}
