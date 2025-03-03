// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRSocketActor.h"
#include <Components/LeeXRMeshSocket.h>
#include <Definitions.h>

// Sets default values
ALeeXRSocketActor::ALeeXRSocketActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletonBasic = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonBasic"));
	SetRootComponent(SkeletonBasic);


}

// Called when the game starts or when spawned
void ALeeXRSocketActor::BeginPlay()
{
	Super::BeginPlay();
	
	TArray< ULeeXRMeshSocket*> MeshSockets{};
	GetComponents<ULeeXRMeshSocket>(MeshSockets);
	if (MeshSockets.IsEmpty()) return;

	for (auto comp : MeshSockets)
	{
		if (comp->OnComponentBeginOverlap.IsBound()) comp->OnComponentBeginOverlap.Clear();
		comp->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRSocketActor::OnBeginOverlap);

		if (comp->OnComponentEndOverlap.IsBound()) comp->OnComponentEndOverlap.Clear();
		comp->OnComponentEndOverlap.AddDynamic(this, &ALeeXRSocketActor::OnEndOverlap);
	}
}

void ALeeXRSocketActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

#if WITH_EDITOR
void ALeeXRSocketActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ALeeXRSocketActor, PreviewColor))
	//{
	//	//Do something
	//	TArray<USceneComponent*> Children;
	//	GetComponents(Children,true);
	//}
}

#endif

void ALeeXRSocketActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (SweepResult.HasValidHitObjectHandle())
	{

		ULeeXRMeshSocket* MeshSocket = CastChecked<ULeeXRMeshSocket>(OverlappedComp);
		MeshSocket->MaterialIns->GetScalarParameterValue(*MeshSocket->MaterialParamName, MeshSocket->DefaultParamValue);
		if (MeshSocket && MeshSocket->IsSimulatingPhysics())
		{
			//Snap to Target
			//LeeScreenLog("Visibility %s", FColor::Green, *OtherActor->GetName());
			OtherActor->AttachToComponent(SkeletonBasic, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MeshSocket->GetAttachSocketName());
			//Change Visibility
			SkeletonBasic->SetVisibility(false);

		}
		else {
			//set Color to Green Correct to Snap
			MeshSocket->MaterialIns->SetScalarParameterValue(*MeshSocket->MaterialParamName, 1.f);
			LeeScreenLog("Changed %s", FColor::Green, *OtherActor->GetName());

		}

	}

}

void ALeeXRSocketActor::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ULeeXRMeshSocket* MeshSocket = CastChecked<ULeeXRMeshSocket>(OverlappedComp);
	MeshSocket->MaterialIns->SetScalarParameterValue(*MeshSocket->MaterialParamName, MeshSocket->DefaultParamValue);
	LeeScreenLog("End Overlap %s", FColor::Red, *OtherActor->GetName());
}
