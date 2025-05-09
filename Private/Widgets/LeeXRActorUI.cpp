// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LeeXRActorUI.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Common/LeeXRUltils.h"
#include "Common/Definitions.h"
#include <Actors/LeeXRPhysicsGrabableObject.h>
#include <Components/WidgetComponent.h>
#include <Actors/LeeXRHandBase.h>


using namespace LeeXRUltils;

// Sets default values
ALeeXRActorUI::ALeeXRActorUI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UStaticMeshComponent* ALeeXRActorUI::FindComponentByName(FString inName)
{
	TArray<UActorComponent*> Components = K2_GetComponentsByClass(UStaticMeshComponent::StaticClass());


	for (auto Component : Components)
	{
		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component);
		if (StaticMeshComponent && StaticMeshComponent->GetName() == inName)
		{
			return StaticMeshComponent;
		}
	}

	return nullptr;
}

UStaticMeshComponent* ALeeXRActorUI::FindCarComponentByName(const FString inCarName)
{

	if (CarComponents.IsEmpty()) return nullptr;
	for (auto Component : CarComponents)
	{
		// Check if the component name matches the given name
		if (Component->GetName() == inCarName ||
			Component->GetName().EndsWith(inCarName) ||
			Component->GetName().StartsWith(inCarName))
		{
			return Component;
		}
	}
	return nullptr;
}

UMaterialInstanceDynamic* ALeeXRActorUI::FindMaterialByName(const FString inName)
{
	if (CarComponents.IsEmpty()) return nullptr;
	if (auto Material = FindCarComponentByName(inName))
	{
		// Check if the component name matches the given name
		if (Material->GetMaterial(0))
		{
			return Cast<UMaterialInstanceDynamic>(Material->GetMaterial(0));
		}
	}
	return nullptr;
}

float ALeeXRActorUI::PlayAnimation()
{
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents(SkeletalMeshComponents);
	if (SkeletalMeshComponents.IsEmpty()) return 0.f;

	for (auto Skeletal : SkeletalMeshComponents)
	{
		Skeletal->Play(false);
	}

	if (auto AnimationAsset = SkeletalMeshComponents[0]->AnimationData.AnimToPlay)
	{
		return AnimationAsset->GetPlayLength();
	}

	return 0.0f;
}

void ALeeXRActorUI::StopAnimation()
{
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents(SkeletalMeshComponents);
	if (SkeletalMeshComponents.IsEmpty()) return;

	for (auto Skeletal : SkeletalMeshComponents)
	{
		Skeletal->Stop();
	}
}

// Called when the game starts or when spawned
void ALeeXRActorUI::BeginPlay()
{
	Super::BeginPlay();
	
	//Get all car components
	CarComponents = GetComponentsByClass<UStaticMeshComponent>("car");
	
	// Find the static mesh component by name
	HandComponent = FindStaticMeshComponentByName<UBoxComponent>("Hand");
	if (HandComponent)
	{
		HandComponent->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRActorUI::OnHandOverlap);
		HandComponent->OnComponentEndOverlap.AddDynamic(this, &ALeeXRActorUI::OnHandEndOverlap);
		LeeScreenLog("Hand Component Found", FColor::Green);
	}
	// Initialize the dynamic material for all car components
	InitDynamicMaterial();

	if (Box = FindComponentByClass<UBoxComponent>()) {
		// init Overlap
		Box->OnComponentBeginOverlap.AddDynamic(this, &ALeeXRActorUI::OnBeginOverlap);

	}
	// Find Material by name

	if (auto FoundMaterial = FindMaterialByName(TEXT("body"))) {
		// Do something with the found material
		UE_LOG(LogTemp, Warning, TEXT("Found Material: %s"), *FoundMaterial->GetName());
		// Set a color parameter on the material
		//PlayAnimation();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Material not found"));
	}



}

// Initialize the dynamic material for all car components
void ALeeXRActorUI::InitDynamicMaterial()
{
	if (CarComponents.IsEmpty()) return;
	for (auto Component : CarComponents)
		Component->CreateDynamicMaterialInstance(0);
}

void ALeeXRActorUI::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LEE_LOG(LogLeeXRActorUI, Log, "OnBeginOverlap: %s", *OtherActor->GetName());

	if (auto Grabable = Cast<ALeeXRPhysicsGrabableObject>(OtherActor))
	{
		FGameplayTag TagGrabable = Grabable->GetTagElemental();

		// Check if the tag is valid
		FString MaterialName;

		if (Grabable->IsTagString("Car.Left",false) && !bLeftReady)
		{
			// Set the custom depth stencil value
			LeeScreenLog("Car Left Completed", FColor::Green);
			bLeftReady = true;
			MaterialName = "left";
		}
		else if (Grabable->IsTagString("Car.Right", false) && !bRightReady)
		{
			// Set the custom depth stencil value
			LeeScreenLog("Car Right Completed", FColor::Green);

			bRightReady = true;
			MaterialName = "right";
		}
		else if (Grabable->IsTagString("Car.Body", false) && !bBodyReady)
		{
			// Set the custom depth stencil value
			LeeScreenLog("Car Body Completed", FColor::Green);
			bBodyReady = true;
			MaterialName = "body";
		}

		if (MaterialName.IsEmpty()) return;

		Grabable->TurnOffRotation();
		//Set Color Completed 
		if (auto FoundMaterial = FindMaterialByName(MaterialName))
		{
			FoundMaterial->SetVectorParameterValue(FName("Color"), FLinearColor::Blue);
		}

		if (bBodyReady && bLeftReady && bRightReady)
		{
			//All Completed
			LeeScreenLog("Car All Completed", FColor::Green);


		}
	}
}

void ALeeXRActorUI::OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Hand Checking
		//Hand Checking
	if (auto Hand = Cast<ALeeXRPhysicsGrabableObject>(OtherActor))
	{
		float rate = UKismetMathLibrary::Lerp(0.f, 1.f, HandVeryfied);

		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(
				HandTimerHandle,
				[this, World]() {

					HandVeryfied = UKismetMathLibrary::FInterpTo(HandVeryfied, 0.f, World->GetDeltaSeconds(), 10.f);
					LeeScreenLog("Hand Veryfied %f", FColor::Green, HandVeryfied);
					//GetWorld()->GetTimerManager().ClearTimer(HandTimerHandle);
				},
				rate,
				true
			);
		}
	}
}

void ALeeXRActorUI::OnHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (auto Hand = Cast<ALeeXRPhysicsGrabableObject>(OtherActor))
	{
		if (HandVeryfied >= 1.f)
		{
			if (!IsReadyToPlay()) return;
			//Play Sound
			float timeCompleted = PlayAnimation();

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, &TimerHandle]()
				{
					if (auto Widget = FindComponentByClass<UWidgetComponent>())
					{
						Widget->SetVisibility(true);
					}
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				},
				1.f,
				false,
				timeCompleted
			);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(HandTimerHandle);
		}
	}
}



// Called every frame
void ALeeXRActorUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

