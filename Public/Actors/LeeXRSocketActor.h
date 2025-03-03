// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRSocketActor.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LeeXRSocketActor, Log, All);

class UWidgetComponent;

UCLASS()
class LEEMETAXRM_API ALeeXRSocketActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletonBasic;


public:	
	// Sets default values for this actor's properties
	ALeeXRSocketActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties", meta = (AllowPrivateAccess="true",DefaultValue="FColor::Green"))
	FColor PreviewColor=FColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	TObjectPtr<UWidgetComponent> StatusAct;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	template<typename T>
	void LookAtComponent(T* TargetComponent, bool isYawOnly);

};

template<typename T>
inline void ALeeXRSocketActor::LookAtComponent(T* TargetComponent, bool isYawOnly)
{
	/// <summary>
	/// Look at the target component
	{
		if (TargetComponent == nullptr) return;

		//// Get the world location of the display component
		FVector DisplayLocation = TargetComponent->GetComponentLocation();

		// Get the world location of the player's camera
		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();

		// Calculate the rotation needed to look at the camera
		FRotator FLookAtRot = UKismetMathLibrary::FindLookAtRotation(DisplayLocation, CameraLocation);

		FRotator LookAtRotation = isYawOnly == false ?
			FLookAtRot :
			FRotator(0, FLookAtRot.Yaw, 0);

		// Set the world rotation of the display component to the calculated rotation
		TargetComponent->SetWorldRotation(FRotator(0, LookAtRotation.Yaw, 0));
	}
}