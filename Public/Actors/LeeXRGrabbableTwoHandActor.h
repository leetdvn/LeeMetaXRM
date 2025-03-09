// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameplayTagContainer.h>
#include "LeeXRGrabbableActor.h"
#include "Interfaces/LeeXRInteraction.h"
#include "CoreMinimal.h"
#include "LeeXRGrabbableTwoHandActor.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LogLeeXRGrabbableTwoHandActor, Log, All)



/**
 * 
 */
UCLASS()
class LEEMETAXRM_API ALeeXRGrabbableTwoHandActor : public ALeeXRGrabbableActor
{
	GENERATED_BODY()
	
public:
	ALeeXRGrabbableTwoHandActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	TObjectPtr<class USkeletalMeshComponent> RootSkeletal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	TObjectPtr<class USphereComponent> MainGribCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	TObjectPtr<class USphereComponent> SecondaryGribCollision;

	virtual void InitSettings() override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void OnGrab(UObject* inComponent, const FVector& InGrabLocation) override;
	virtual void OnRelease(UObject* inComponent) override;
	virtual void OnGrabObjects(UMotionControllerComponent* inComponent) override;
	virtual void OnReleaseObjects(UMotionControllerComponent* inComponent) override;

private:

};
