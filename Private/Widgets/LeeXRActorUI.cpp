// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LeeXRActorUI.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

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

void ALeeXRActorUI::PlayAnimation()
{
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents(SkeletalMeshComponents);
	if (SkeletalMeshComponents.IsEmpty()) return;

	for (auto Skeletal : SkeletalMeshComponents)
	{
		Skeletal->Play(false);
	}
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
		PlayAnimation();
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

}



// Called every frame
void ALeeXRActorUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

