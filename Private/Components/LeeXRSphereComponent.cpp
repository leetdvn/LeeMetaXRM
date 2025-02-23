// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/LeeXRSphereComponent.h"
#include "Common/Definitions.h"

ULeeXRSphereComponent::ULeeXRSphereComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SphereRadius = 1.0f;
}

void ULeeXRSphereComponent::OnHitComp(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	LeeScreenLog("Hit Info : %s", FColor::Green,*HitComponent->GetName());
}

void ULeeXRSphereComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &ULeeXRSphereComponent::OnBeginOverlap);
}

void ULeeXRSphereComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor->GetName() == GetAttachParentActor()->GetName()) return;
	TArray<AActor*> IgnoreActor{};
	//if(OtherActor->GetName() == ParentAc)
	LeeScreenLog("Overlap Info : %s", FColor::Green, *OtherActor->GetName());
}